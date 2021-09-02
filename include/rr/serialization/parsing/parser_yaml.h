#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>

#include "../../expected.h"
#include "../../reflection/reflection.h"
#include "../../variable/box.h"
#include "define_retry.h"
#include "lexers/compiled/lexer_yaml.yy.h"
#include "rr/variant/variant.h"

namespace rr {

class ParserYaml : public LexerYaml {
 public:
  ParserYaml(const char* input, size_t input_size)  //
      : LexerYaml(reflex::Input(input, input_size)), _token(static_cast<char>(lex())) {
  }

  explicit ParserYaml(std::istream& stream)  //
      : LexerYaml(stream), _token(static_cast<char>(lex())) {
  }

  Expected<None> deserialize(TypeInfo* info) {
    while (true) {
      if (_token == 'S') {
        next();
        continue;
      }
      if (is_end(_token)) {
        // break in the end of one document even if there are few in a file
        return Error("Unexpected end of file");
      }
      return parse(info);
    }
  }

 private:
  Expected<None> parse(TypeInfo* info) {
    if (is_new_line(_token)) {
      next();
    }

    // just ignore tags
    if (_token == '!') {
      next();
    }

    if (_token == '*') {
      auto anchor = get_word();
      auto var = _anchors[anchor.substr(1, anchor.size() - 1)];
      Reflection::copy(info->var(), var);

      next();
      return None();
    }

    std::string anchor;
    if (_token == '&') {
      anchor = get_word();
      next();
    }

    Expected<None> ex = None();
    switch (_token) {
      case '-':
        ex = parse_seq(info);
        break;
      case '>':
        ex = parse_ind(info);
        break;
      case '$':
        ex = parse_str(info);
        break;
      case '[':
        ex = info->match(
            [this](Array& a) -> Expected<None> {
              return parse_flow_seq(a.nested_type(), [&](size_t i, Var var) { return add_to_array(a, i, var); });
            },
            [this](Sequence& s) -> Expected<None> {
              return parse_flow_seq(s.nested_type(), [&](size_t, Var var) { return s.push(var); });
            },
            [this](auto&&) -> Expected<None> { return error_match(); });
        break;
      case '{':
        ex =
            info->match([this](Object& o) -> Expected<None> { return parse_flow_map([&]() { return add_to_obj(o); }); },
                        [this](Map& m) -> Expected<None> { return parse_flow_map([&]() { return add_to_map(m); }); },
                        [this](auto&&) -> Expected<None> { return error_match(); });
        break;
      case '?':
        ex = info->match([this](Map& m) -> Expected<None> { return parse_flow_map([&]() { return add_to_map(m); }); },
                         [this](auto&&) -> Expected<None> {
                           return error("A complex key, marked '?' could be deserialized in a map key only");
                         });
        break;
      default:
        ex = error_token(_token);
        break;
    }

    if (!anchor.empty() && anchor[0] == '&') {
      _anchors[anchor.substr(1, anchor.size() - 1)] = info->var();
    }

    return ex;
  }

  // parse "- val ..."
  Expected<None> parse_seq(TypeInfo* info) {
    return info->match(
        [this](Array& a) -> Expected<None> {
          return parse_seq(a.nested_type(), [&](size_t i, Var var) { return add_to_array(a, i, var); });
        },
        [this](Sequence& s) -> Expected<None> {
          return parse_seq(s.nested_type(), [&](size_t, Var var) { return s.push(var); });
        },
        [this](auto&&) -> Expected<None> { return error_match(); });
  }

  Expected<None> parse_seq(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add) {
    if (_token == '[') {
      return parse_flow_seq(nested_type, std::move(add));
    }

    size_t level = 0;
    size_t i = 0;

    Box box(nested_type);
    auto info = Reflection::reflect(box.var());

    while (true) {
      if (_token == '>') {
        next();
        ++level;
      }
      while (_token == '<') {
        if (level == 0) {
          return None();
        }
        next();
        if (level == 1) {
          return None();
        }
        --level;
      }
      if (_token != '-') {
        return error_token(_token);
      }

      next();  // skip '-' itself

      __retry(parse(&info));
      i++;
      __retry(add(i, box.var()));
    }
    return None();
  }

  // parse "[ val, ... ]"
  Expected<None> parse_flow_seq(TypeId nested_type, std::function<Expected<None>(size_t, Var)> add) {
    next();  // skip '[' itself

    size_t level = 0;
    size_t i = 0;

    Box box(nested_type);
    auto info = Reflection::reflect(box.var());

    while (!is_end(_token) && _token != 'S' && _token != ']') {
      if (is_new_line(_token)) {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0) {
            --level;
          }
          next();
        }
      }

      __retry(parse(&info));
      i++;
      __retry(add(i, box.var()));

      if (is_new_line(_token)) {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0) {
            --level;
          }
          next();
        }
      }
      if (_token == ',') {
        next();
      }
      if (is_new_line(_token)) {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0) {
            --level;
          }
          next();
        }
      }
    }
    if (_token == ']') {
      next();
    }
    if (is_new_line(_token)) {
      next();
    }
    while (_token == '<' && level-- > 0) {
      next();
    }
    return None();
  }

  static inline Expected<None> add_to_array(Array& a, size_t i, Var var) {
    if (i < a.size()) {
      auto item = a.at(i).unwrap();
      Reflection::copy(item, var);
    }
    return None();
  }

  // parse indented value (string, nested sequence, or nested map)
  Expected<None> parse_ind(TypeInfo* info) {
    next();  // skip indention token itself

    auto ex = parse(info);

    if (is_new_line(_token)) {
      next();
    }
    if (_token == '<') {
      next();
    }
    return ex;
  }

  // parse string of "key: val ..."
  Expected<None> parse_str(TypeInfo* info) {
    // a string could be a key in a map
    // or name of a field in an object
    // or be just a string value of something
    auto ex = info->match([this](Bool& b) -> Expected<None> { return b.set(parse_bool(get_word())); },
                          [this](Integer& i) -> Expected<None> { return i.set(parse_int(get_word())); },
                          [this](Floating& f) -> Expected<None> { return f.set(parse_double(get_word())); },
                          [this](String& s) -> Expected<None> { return s.set(get_word()); },
                          [this](Enum& e) -> Expected<None> { return e.parse(get_word()); },
                          [this](Map& m) -> Expected<None> { return parse_map([&]() { return add_to_map(m); }); },
                          [this](Object& o) -> Expected<None> { return parse_map([&]() { return add_to_obj(o); }); },
                          [this](auto&&) -> Expected<None> { return error_match(); });
    __retry(ex);
    next();

    if (is_new_line(_token) || is_end(_token)) {
      next();
    }

    return ex;
  }

  Expected<None> parse_map(std::function<Expected<None>()> add) {
    if (_token == '{') {
      return parse_flow_map(std::move(add));
    }

    while (_token != 'S' && !is_end(_token) && _token != '<') {

      if (is_new_line(_token) || _token == '$') {
        next();
      }

      if (_token == ':') {
        __retry(add());
      } else {
        return error_token(_token);
      }
    }
    return None();
  }

  // parse "{ key:val, ... }" to an object
  Expected<None> parse_flow_map(std::function<Expected<None>()> add) {
    next();  // skip '{' itself

    size_t level = 0;
    while (_token != 'S' && !is_end(_token) && _token != '}') {

      if (is_new_line(_token) || _token == '$') {
        next();
      }

      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0) {
            --level;
          }
          next();
        }
      }

      if (_token == ':') {
        __retry(add());
      } else {
        return error_token(_token);
      }

      if (is_new_line(_token)) {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0) {
            --level;
          }
          next();
        }
      }
      if (_token == ',') {
        next();
      }
      if (is_new_line(_token)) {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else if (_token == '<') {
        if (level > 0) {
          --level;
        }
        next();
      }
    }

    if (_token == '}') {
      next();
    }
    if (is_new_line(_token)) {
      next();
    }
    while (_token == '<' && level-- > 0) {
      next();
    }

    return None();
  }

  inline Expected<None> add_to_obj(Object& obj) {
    auto ex = obj.get_field(get_word());
    __retry(ex);

    auto info = Reflection::reflect(ex.unwrap());

    next();
    __retry(parse(&info));
    return None();
  }

  inline Expected<None> add_to_map(Map& map) {
    Box key_box(map.key_type());
    Box val_box(map.val_type());

    auto info_k = Reflection::reflect(key_box.var());
    auto info_v = Reflection::reflect(val_box.var());

    // get a key
    __retry(parse(&info_k));
    next();

    // get a value
    __retry(parse(&info_v));
    if (_token == '<') {
      next();
    }

    map.insert(key_box.var(), val_box.var());
    return None();
  }

  char next() {
    if (_token != 0) {
      _token = static_cast<char>(lex());
    }
    return _token;
  }

  static inline bool is_end(int token) {
    return token == 0 || token == 'E';
  }

  static inline bool is_new_line(int token) {
    return token == '=' || token == ';';
  }

  inline Error error(const char* str) {
    return Error(format("{}; {}", str, get_position().to_string()));
  }

  inline Error error_token(char token) {
    return Error(format("Unexpected token '{}'; {}", token, get_position().to_string()));
  }

  inline Error error_match() {
    return Error(format("Cannot match correct type; {}", get_position().to_string()));
  }

  static inline bool parse_bool(const std::string& str) {
    if (str.size() > 5) {  // 5 is length of the longest variant 'false'
      return true;
    }

    std::string t = str;
    std::transform(t.begin(), t.end(), t.begin(), [](char c) { return std::tolower(c); });

    return !(t == "false" || t == "off" || t == "no" || t == "n");
  }

  static inline int64_t parse_int(const std::string& str) {
    return std::strtoll(&str[0], nullptr, 10);
  }

  static inline double parse_double(const std::string& str) {
    return std::strtod(&str[0], nullptr);
  }

  char _token;
  std::unordered_map<std::string, Var> _anchors;
};

}  // namespace rr

#undef __retry