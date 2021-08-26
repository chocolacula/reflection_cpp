#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../../error/error_parse.h"
#include "../../reflection/reflection.h"
#include "../../variable/box.h"
#include "lexers/compiled/lexer_json.yy.h"
#include "rr/expected.h"
#include "rr/info/type_id.h"
#include "rr/tools/format.h"
#include "rr/variant/variant.h"

#define RET(x)                      \
  {                                 \
    auto x__ = std::move(x);        \
    if (x__.is_error()) {           \
      return x__.get<ErrorParse>(); \
    }                               \
  }

namespace rr {

class ParserJson : public LexerJson {
 public:
  ParserJson(const char* input, size_t input_size) : LexerJson(reflex::Input(input, input_size)), _level(0) {
  }

  ParserJson(std::istream& stream) : LexerJson(stream), _level(0) {
  }

  Expected<None, ErrorParse> deserialize(TypeInfo* info) {
    auto result = parse(info);
    return result.match_move([](Kind&& k) -> Expected<None, ErrorParse> { return None(); },
                             [](ErrorParse&& err) -> Expected<None, ErrorParse> { return err; });
  }

 private:
  enum class Kind { kScalar, kArray, kObject, kBracketR };

  Expected<Kind, ErrorParse> parse(TypeInfo* info) {
    int token = lex();
    switch (token) {
      case '0':
        // do nothing
        return Kind::kScalar;
      case 't':
        info->get<Bool>().set(true);
        return Kind::kScalar;
      case 'f':
        info->get<Bool>().set(false);
        return Kind::kScalar;
      case '#':
        info->match([this](Integer& i) { i.set(parse_int(string)); },   //
                    [this](Float& f) { f.set(parse_double(string)); },  //
                    [](auto&&) {});
        return Kind::kScalar;
      case '$':
        info->match([this](String& s) { s.set(string); },  //
                    [this](Enum& e) { e.parse(string); },  //
                    [](auto&&) {});
        return Kind::kScalar;
      case '[':
        return parse_array(info).match_move(  //
            [](None&&) -> Expected<Kind, ErrorParse> { return Kind::kArray; },
            [](ErrorParse&& err) -> Expected<Kind, ErrorParse> { return err; });
      case '{':
        return parse_object(info).match_move(  //
            [](None&&) -> Expected<Kind, ErrorParse> { return Kind::kObject; },
            [](ErrorParse&& err) -> Expected<Kind, ErrorParse> { return err; });
      default:
        if (token == ']') {
          if (_level > 0) {
            return Kind::kBracketR;
          }
          return ErrorParse("Unexpected ']' without '['", get_position());
        }
        return ErrorParse(format("Unexpected token '{}'", token), get_position());
    }
  }

  Expected<None, ErrorParse> parse_array(TypeInfo* info) {
    if (++_level > kMaxLevel) {
      return ErrorParse("Max depth level exceeded", get_position());
    }

    return info->match(
        [this](Array& a) -> Expected<None, ErrorParse> {
          auto size = a.size() < kMaxArr ? a.size() : kMaxArr;
          auto type = a.nested_type();

          return parse_items(size, type, [&a](size_t i, Var value) {
            auto item = a.at(i).unwrap();
            Reflection::copy(item, value);
          });
        },
        [this](Sequence& s) -> Expected<None, ErrorParse> {
          auto type = s.nested_type();
          return parse_items(kMaxArr, type, [&s](size_t, Var value) { s.push(value); });
        },
        [this](Map& m) -> Expected<None, ErrorParse> { return parse_map(m); },
        [this](auto&&) -> Expected<None, ErrorParse> {
          return ErrorParse("TypeInfo isn't fit for array parsing", get_position());
        });
  }

  Expected<None, ErrorParse> parse_items(size_t max, TypeId type, std::function<void(size_t, Var)> add) {
    for (size_t len = 0; len < max; ++len) {

      Box box(type);
      auto boxed_info = Reflection::reflect(box.var());

      auto exp = parse(&boxed_info)
                     .match_move(
                         [&](Kind&& k) -> Expected<None, ErrorParse> {
                           if (k == Kind::kBracketR) {
                             _level -= 1;
                             if (len == 0) {
                               // TODO check it, possibly an empty array
                               return None();
                             }
                             return ErrorParse("Unexpected end of the array", get_position());
                           }
                           add(len, box.var());
                           return None();
                         },
                         [](ErrorParse&& err) -> Expected<None, ErrorParse> { return err; });
      RET(exp);

      int token = lex();
      if (token == ']') {
        --_level;
        return None();
      }
      if (token != ',') {
        return ErrorParse(format("Unexpected token '{}'", token), get_position());
      }
    }

    return None();
  }

  Expected<None, ErrorParse> parse_object(TypeInfo* info) {
    if (++_level > kMaxLevel) {
      return ErrorParse("Max depth level exceeded", get_position());
    }

    for (size_t len = 0; len < kMaxArr; ++len) {
      int token = lex();

      if (len == 0 && token == '}') {
        --_level;
        return None();
      }
      if (token != '$') {
        return ErrorParse("Cannot reach a field name", get_position());
      }
      if (lex() != ':') {
        return ErrorParse("Cannot reach a field value", get_position());
      }

      auto field = info->get<Object>().get_field(string).unwrap();
      RET(parse_field(field));

      token = lex();
      if (token == '}') {
        --_level;
        return None();
      }
      if (token != ',') {
        return ErrorParse(format("Unexpected token '{}'", token), get_position());
      }
    }

    return ErrorParse("Max depth level exceeded", get_position());
  }

  Expected<None, ErrorParse> parse_map(Map& map) {
    if (_level > kMaxLevel) {
      return ErrorParse("Max depth level exceeded", get_position());
    }

    int token = lex();

    std::string key = "key";
    std::string val = "val";

    if (token == '$') {
      // if particular tag found parse it
      auto pos = string.find("!!map");
      if (pos != std::string::npos) {

        auto kv = parse_tag(string);
        RET(kv);

        auto pair = kv.unwrap();
        key = pair.first;
        val = pair.second;

        // make step to get a new token
        token = lex();
      } else {
        return ErrorParse("Cannot reach the map tag or '{'", get_position());
      }
    } else if (token != '{') {
      ErrorParse(format("Unexpected token '{}'", token), get_position());
    }

    for (size_t len = 0; len < kMaxArr; ++len) {
      // token '{' has already been read
      ++_level;

      // parse first field key or val
      token = lex();
      if (token != '$') {
        return ErrorParse("Cannot reach a field name", get_position());
      }

      if (lex() != ':') {
        return ErrorParse("Cannot reach a field value", get_position());
      }

      Box key_box(map.key_type());
      Box val_box(map.val_type());

      if (string == key) {
        RET(parse_field(key_box.var()));
      } else if (string == val) {
        RET(parse_field(val_box.var()));
      } else {
        return ErrorParse(format("Got an unexpected field '{}' while parse map", string), get_position());
      }

      token = lex();
      if (token == '}') {
        --_level;
        return ErrorParse("Unexpected end of JSON object", get_position());
      }
      if (token != ',') {
        return ErrorParse(format("Unexpected token '{}'", token), get_position());
      }

      // parse second field key or val
      token = lex();
      if (token != '$') {
        return ErrorParse("Cannot reach a field name", get_position());
      }

      if (lex() != ':') {
        return ErrorParse("Cannot reach a field value", get_position());
      }

      if (string == key) {
        RET(parse_field(key_box.var()));
      } else if (string == val) {
        RET(parse_field(val_box.var()));
      } else {
        return ErrorParse(format("Got an unexpected field '{}' while parse map", string), get_position());
      }

      auto exp = map.insert(key_box.var(), val_box.var());
      if (exp.is_error()) {
        return ErrorParse(exp.get<Error>().what(), get_position());
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
        return ErrorParse(format("Unexpected token '{}'", token), get_position());
      }

      // take next '{'
      token = lex();
    }

    return ErrorParse("Max depth level exceeded", get_position());
  }

  inline Expected<None, ErrorParse> parse_field(Var new_var) {
    auto info = Reflection::reflect(new_var);

    return parse(&info).match_move(
        [&](Kind&& k) -> Expected<None, ErrorParse> {
          if (k == Kind::kBracketR) {
            return ErrorParse("parse() returned ']' unexpectedly", get_position());
          }
          return None();
        },
        [](ErrorParse&& err) -> Expected<None, ErrorParse> { return err; });
  }

  static inline int64_t parse_int(const std::string& str) {
    return std::strtoll(&str[0], nullptr, 10);
  }

  static inline double parse_double(const std::string& str) {
    return std::strtod(&str[0], nullptr);
  }

  inline Expected<std::pair<std::string, std::string>, ErrorParse> parse_tag(const std::string& str) {
    auto pos1 = str.find('|');
    if (pos1 == std::string::npos) {
      return ErrorParse("Cannot find '|' in the tag", get_position());
    }
    auto pos2 = str.find(':');
    if (pos2 == std::string::npos) {
      return ErrorParse("Cannot find ':' in the tag", get_position());
    }

    pos1 += 1;

    auto key = str.substr(pos1, pos2 - pos1);
    auto val = str.substr(pos2, str.size() - pos2);

    return std::make_pair(std::move(key), std::move(val));
  }

  static const size_t kMaxLevel = 100;  // to keep things secure: max JSON nesting depth
  static const size_t kMaxArr = 1000;   // to keep things secure: max JSON array and object size
  size_t _level;
};

}  // namespace rr

#undef RET