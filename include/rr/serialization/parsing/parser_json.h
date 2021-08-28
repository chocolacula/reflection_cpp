#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../../reflection/reflection.h"
#include "../../variable/box.h"
#include "lexers/compiled/lexer_json.yy.h"

namespace rr {

class ParserJson : public LexerJson {
 public:
  ParserJson(const char* input, size_t input_size) : LexerJson(reflex::Input(input, input_size)), _level(0) {
  }

  ParserJson(std::istream& stream) : LexerJson(stream), _level(0) {
  }

  Expected<None> deserialize(TypeInfo* info) {
    return parse_next(info);
  }

 private:
  Expected<None> parse_next(TypeInfo* info) {
    return parse(info, lex());
  }

  Expected<None> parse(TypeInfo* info, int token) {
    switch (token) {
      case '0':
        // do nothing
        return None();
      case 't':
        return info->get<Bool>().set(true);
      case 'f':
        return info->get<Bool>().set(false);
      case '#':
        return info->match([this](Integer& i) -> Expected<None> { return i.set(parse_int(string)); },
                           [this](Float& f) -> Expected<None> { return f.set(parse_double(string)); },
                           [this](auto&&) -> Expected<None> { return error_match(); });
      case '$':
        return info->match([this](String& s) -> Expected<None> { return s.set(string); },
                           [this](Enum& e) -> Expected<None> { return e.parse(string); },
                           [this](auto&&) -> Expected<None> { return error_match(); });
      case '[':
        // clang-format off
        return info->match(
            [this, info](Array& a) -> Expected<None> {
              return parse_array(a.nested_type(), [this, &a](size_t i, Var var) { add_to_array(a, i, var); });
            },
            [this, info](Sequence& s) -> Expected<None> {
              return parse_array(s.nested_type(), [&s](size_t, Var var) { s.push(var); });
            },
            [this](Map& m) -> Expected<None> {
              return parse_map(m);
            },
            [this](auto&&) -> Expected<None> {
              return error_match();
            });
        // clang-format on
      case '{':
        return parse_object(info);
      default:
        return error_token(token);
    }
  }

  Expected<None> parse_array(TypeId nested_type, std::function<void(size_t, Var)> add) {
    if (++_level > kMaxLevel) {
      return error("Max depth level exceeded");
    }

    int token = lex();
    if (token == ']') {
      // an empty array
      _level -= 1;
      return None();
    }

    for (size_t len = 0; len < kMaxArr; ++len) {

      Box box(nested_type);
      auto boxed_info = Reflection::reflect(box.var());

      auto exp = parse(&boxed_info, token)
                     .match_move(
                         [&, len](None&&) -> Expected<None> {
                           add(len, box.var());
                           return None();
                         },
                         [](Error&& err) -> Expected<None> { return err; });

      if (exp.is_error()) {
        return exp.error();
      }

      int token = lex();
      if (token == ']') {
        --_level;
        return None();
      }
      if (token != ',') {
        return error_token(token);
      }

      // get another one
      token = lex();
    }

    return None();
  }

  static inline void add_to_array(Array& a, size_t i, Var var) {
    if (i >= a.size()) {
      return;
    }
    auto item = a.at(i).unwrap();
    Reflection::copy(item, var);
  }

  Expected<None> parse_object(TypeInfo* info) {
    if (++_level > kMaxLevel) {
      return error("Max depth level exceeded");
    }

    for (size_t len = 0; len < kMaxArr; ++len) {
      int token = lex();

      if (len == 0 && token == '}') {
        --_level;
        return None();
      }
      if (token != '$') {
        return error("Cannot reach a field name");
      }
      if (lex() != ':') {
        return error("Cannot reach a field value");
      }

      auto field = info->get<Object>().get_field(string).unwrap();
      auto exp = parse_field(field);
      if (exp.is_error()) {
        return exp.error();
      }

      token = lex();
      if (token == '}') {
        --_level;
        return None();
      }
      if (token != ',') {
        return error_token(token);
      }
    }

    return error("Max depth level exceeded");
  }

  Expected<None> parse_map(Map& map) {
    if (++_level > kMaxLevel) {
      return error("Max depth level exceeded");
    }

    int token = lex();

    std::string key = "key";
    std::string val = "val";

    if (token == '$') {
      // if particular tag found parse it
      auto pos = string.find("!!map");
      if (pos != std::string::npos) {

        auto kv = parse_tag(string);
        if (kv.is_error()) {
          return kv.error();
        }

        auto pair = kv.unwrap();
        key = std::move(pair.first);
        val = std::move(pair.second);

        // make step to get a new token
        if (lex() != ',') {
          return error_token(token);
        }
        token = lex();
      } else {
        return error("Cannot reach the map tag or '{'");
      }
    } else if (token != '{') {
      return error_token(token);
    }

    for (size_t len = 0; len < kMaxArr; ++len) {
      // token '{' has already been read
      ++_level;

      // parse first field key or val
      token = lex();
      if (token != '$') {
        return error("Cannot reach a field name");
      }

      if (lex() != ':') {
        return error("Cannot reach a field value");
      }

      Box key_box(map.key_type());
      Box val_box(map.val_type());

      if (string == key) {
        auto exp = parse_field(key_box.var());
        if (exp.is_error()) {
          return exp.error();
        }
      } else if (string == val) {
        auto exp = parse_field(val_box.var());
        if (exp.is_error()) {
          return exp.error();
        }
      } else {
        return Error(format("Got an unexpected field '{}' while parse map; {}", string, get_position().to_string()));
      }

      token = lex();
      if (token == '}') {
        --_level;
        return error("Unexpected end of JSON object");
      }
      if (token != ',') {
        return error_token(token);
      }

      // parse second field key or val
      token = lex();
      if (token != '$') {
        return error("Cannot reach a field name");
      }

      if (lex() != ':') {
        return error("Cannot reach a field value");
      }

      if (string == key) {
        auto exp = parse_field(key_box.var());
        if (exp.is_error()) {
          return exp.error();
        }
      } else if (string == val) {
        auto exp = parse_field(val_box.var());
        if (exp.is_error()) {
          return exp.error();
        }
      } else {
        return Error(format("Got an unexpected field '{}' while parse map; {}", string, get_position().to_string()));
      }

      auto exp = map.insert(key_box.var(), val_box.var());
      if (exp.is_error()) {
        return exp.error();
      }

      token = lex();
      if (token == '}') {
        --_level;
        token = lex();
      }
      if (token == ']') {
        --_level;
        return None();
      }
      if (token != ',') {
        return error_token(token);
      }

      // take next '{'
      token = lex();
    }

    return error("Max depth level exceeded");
  }

  inline Error error(const char* str) {
    return Error(format("{}; {}", str, get_position().to_string()));
  }

  inline Error error_token(int token) {
    return Error(format("Unexpected token '{}'; {}", token, get_position().to_string()));
  }

  inline Error error_match() {
    return Error(format("Cannot match correct type; {}", get_position().to_string()));
  }

  inline Expected<None> parse_field(Var new_var) {
    auto info = Reflection::reflect(new_var);
    return parse_next(&info);
  }

  inline Expected<std::pair<std::string, std::string>> parse_tag(const std::string& str) {
    auto pos1 = str.find('|');
    if (pos1 == std::string::npos) {
      return error("Cannot find '|' in the tag");
    }
    auto pos2 = str.find(':');
    if (pos2 == std::string::npos) {
      return error("Cannot find ':' in the tag");
    }

    auto key = str.substr(pos1 + 1, pos2 - (pos1 + 1));
    auto val = str.substr(pos2 + 1, str.size() - (pos2 + 1));

    return std::make_pair(std::move(key), std::move(val));
  }

  static inline int64_t parse_int(const std::string& str) {
    return std::strtoll(&str[0], nullptr, 10);
  }

  static inline double parse_double(const std::string& str) {
    return std::strtod(&str[0], nullptr);
  }

  static const size_t kMaxLevel = 100;  // to keep things secure: max JSON nesting depth
  static const size_t kMaxArr = 1000;   // to keep things secure: max JSON array and object size
  size_t _level;
};

}  // namespace rr
