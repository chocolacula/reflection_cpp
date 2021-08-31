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
#include "rr/info/type_info/variants/array/array.h"
#include "rr/info/type_info/variants/enum/enum.h"
#include "rr/info/type_info/variants/integer.h"
#include "rr/info/type_info/variants/map/map.h"
#include "rr/info/type_info/variants/object.h"
#include "rr/info/type_info/variants/sequence/sequence.h"
#include "rr/info/type_info/variants/string.h"
#include "rr/reflection/printer.h"

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

    // anchors support
    if (_token == '&') {
      // TODO get name of anchor
      next();
    } else if (_token == '*') {
      // TODO find a var with the same name and copy it
      next();
    }

    switch (_token) {
      case '-':
        return parse_seq(info);
      case '>':
        return parse_ind(info);
      case '$':
        return parse_str(info);
      case '[':
        return parse_flow_seq(info);
      case '{':
        return parse_flow_map(info);
      case '?':
        return parse_key(info);
      default:
        return error_token(_token);
    }
  }

  // parse "? key : val ..."
  Expected<None> parse_key(TypeInfo* info) {
    next();

    if (!info->is<Map>()) {
      return error_match();
    }

    auto m = info->get<Map>();

    Box key_box(m.key_type());

    auto info_k = Reflection::reflect(key_box.var());
    if (is_new_line(_token)) {
      next();

      if (_token == '>') {
        __retry(parse_ind(&info_k));
      }
    } else {
      matcher().insert_stop(matcher().columno());
      __retry(parse(&info_k));

      if (_token == '<') {
        next();
      }
    }

    if (_token == ':') {
      // parse all other data
      // return parse_map_to_map(m, key_box.var());
    }

    return error("Cannot reach a value while deserialize map");
  }

  // parse "- val ..."
  Expected<None> parse_seq(TypeInfo* info) {
    size_t level = 0;
    size_t i = 0;

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
      i++;

      auto ex = info->match(
          [this, i](Array& a) -> Expected<None> {
            if (i >= a.size()) {
              return None();
            }

            Box box(a.nested_type());
            auto box_info = Reflection::reflect(box.var());

            __retry(parse(&box_info));

            auto item = a.at(i).unwrap();

            Reflection::copy(item, box.var());
            return None();
          },
          [this](Sequence& s) -> Expected<None> {
            Box box(s.nested_type());
            auto box_info = Reflection::reflect(box.var());

            __retry(parse(&box_info));

            s.push(box.var());
            return None();
          },
          [this](auto&&) -> Expected<None> { return error_match(); });
      __retry(ex);
    }
    return None();
  }

  // parse "[ val, ... ]"
  Expected<None> parse_flow_seq(TypeInfo* info) {
    next();

    size_t level = 0;
    size_t i = 0;

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

      auto ex = info->match(
          [this, i](Array& a) -> Expected<None> {
            if (i >= a.size()) {
              return None();
            }

            Box box(a.nested_type());
            auto box_info = Reflection::reflect(box.var());

            __retry(parse(&box_info));

            auto item = a.at(i).unwrap();

            Reflection::copy(item, box.var());
            return None();
          },
          [this](Sequence& s) -> Expected<None> {
            Box box(s.nested_type());
            auto box_info = Reflection::reflect(box.var());

            __retry(parse(&box_info));

            s.push(box.var());
            return None();
          },
          [this](auto&&) -> Expected<None> { return error_match(); });
      __retry(ex);

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

  // parse indented value (string, nested sequence, or nested map)
  Expected<None> parse_ind(TypeInfo* info) {
    next();  // skip indention token itself

    auto ex = info->match([this, info](Array& a) -> Expected<None> { return parse_seq(info); },
                          [this, info](Sequence& s) -> Expected<None> { return parse_seq(info); },
                          [this](String& s) -> Expected<None> { return s.set(string); },
                          [this](Object& o) -> Expected<None> { return parse_map2(o); },
                          [this](Map& m) -> Expected<None> { return parse_map2(m); },
                          [this](auto&&) -> Expected<None> { return error_match(); });
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
    auto ex = info->match([this](Bool& b) -> Expected<None> { return b.set(parse_bool(string)); },
                          [this](Integer& i) -> Expected<None> { return i.set(parse_int(string)); },
                          [this](Float& f) -> Expected<None> { return f.set(parse_double(string)); },
                          [this](String& s) -> Expected<None> { return s.set(string); },
                          [this](Enum& e) -> Expected<None> { return e.parse(string); },
                          [this](Map& m) -> Expected<None> { return parse_map2(m); },
                          [this](Object& o) -> Expected<None> { return parse_map2(o); },
                          [this](auto&&) -> Expected<None> { return error_match(); });
    __retry(ex);
    next();

    if (is_new_line(_token) || is_end(_token)) {
      next();
    }
    return ex;
  }

  Expected<None> parse_map2(Object& obj) {
    while (_token != 'S' && !is_end(_token) && _token != '<') {

      if (is_new_line(_token) || _token == '$') {
        next();
      }

      if (_token == ':') {

        auto ex = obj.get_field(string);
        __retry(ex);

        auto info = Reflection::reflect(ex.unwrap());

        next();
        if (is_new_line(_token)) {
          next();
          if (_token == '-') {
            __retry(parse_seq(&info));
          } else if (_token == '>') {
            __retry(parse_ind(&info));
          }
        } else {
          matcher().insert_stop(matcher().columno());

          __retry(parse(&info));

          if (_token == '<') {
            next();
          }
        }
      } else {
        return error_token(_token);
      }
    }
    return None();
  }

  Expected<None> parse_map2(Map& map) {
    while (_token != 'S' && !is_end(_token) && _token != '<') {

      if (is_new_line(_token)) {
        next();
      }

      Box key_box(map.key_type());
      Box val_box(map.val_type());

      auto info_k = Reflection::reflect(key_box.var());
      auto info_v = Reflection::reflect(val_box.var());

      // get a key
      if (_token == '$') {
        next();
        if (_token == ';') {
          next();
        }
        auto ex = info_k.match([this](Bool& b) -> Expected<None> { return b.set(parse_bool(string)); },
                               [this](Integer& i) -> Expected<None> { return i.set(parse_int(string)); },
                               [this](Float& f) -> Expected<None> { return f.set(parse_double(string)); },
                               [this](String& s) -> Expected<None> { return s.set(string); },
                               [this](Enum& e) -> Expected<None> { return e.parse(string); },
                               [this](auto&&) -> Expected<None> { return error_match(); });
        __retry(ex);

      } else if (_token == '?') {
        next();
        if (is_new_line(_token)) {
          next();
          if (_token == '-') {
            __retry(parse_seq(&info_k));
          } else if (_token == '>') {
            __retry(parse_ind(&info_k));
          }
        } else {
          // matcher().insert_stop(matcher().columno());
          __retry(parse(&info_k));
          if (_token == '<') {
            next();
          }
        }
      } else {
        return error_token(_token);
      }

      // get a value
      if (_token == ':') {
        next();

        if (is_new_line(_token)) {
          next();
          if (_token == '-') {
            __retry(parse_seq(&info_v));
          } else if (_token == '>') {
            __retry(parse_ind(&info_v));
          }
        } else {
          matcher().insert_stop(matcher().columno());

          __retry(parse(&info_v));

          if (_token == '<') {
            next();
          }
        }
      } else {
        return error_token(_token);
      }

      map.insert(key_box.var(), val_box.var());
    }
    return None();
  }

  // key is given in data, now parse ": val ..."
  /*Expected<None> parse_map(TypeInfo* info) {

    {
      auto ex = info->get<Object>().get_field(string);
      __retry(ex);

      auto info_f = Reflection::reflect(ex.unwrap());

      next();

      if (is_new_line(_token)) {
        next();
        if (_token != 'S' && !is_end(_token) && _token != '$' &&  //
            _token != '?' && _token != ':' && _token != '<') {
          // the key has been already consumed, parse a value from new line
          __retry(parse(&info_f));
        }
      } else if (_token != 'S' && !is_end(_token) &&  //
                 _token != '?' && _token != ':' && _token != '<') {
        // the key has been already consumed, parse a value in the same line
        __retry(parse(&info_f));
      }
    }

    // add all other keys and values
    while (_token != 'S' && !is_end(_token) && _token != '<') {

      if (is_new_line(_token)) {
        next();
      }

      // get a key
      std::string key;
      if (_token == '$') {
        next();
        if (_token == ';') {
          next();
        }
        key = string;
      } else {
        return error("Only string keys are supported while parse a yaml map to an object");
      }

      // get a value
      if (_token == ':') {

        auto ex = info->get<Object>().get_field(key);
        __retry(ex);

        auto info_f = Reflection::reflect(ex.unwrap());

        next();
        if (is_new_line(_token)) {
          next();
          if (_token == '-') {
            __retry(parse_seq(&info_f));
          } else if (_token == '>') {
            __retry(parse_ind(&info_f));
          }
        } else {
          matcher().insert_stop(matcher().columno());

          __retry(parse(&info_f));

          if (_token == '<') {
            next();
          }
        }
      } else {
        break;
      }
    }
    return None();
  }

  // key is given in data, now parse ": val ..."
  Expected<None> parse_map_to_map(Map& map, Var key) {
    next();

    Box val_box(map.val_type());
    auto info_v = Reflection::reflect(val_box.var());

    if (is_new_line(_token)) {
      next();
      if (_token != 'S' && !is_end(_token) && _token != '$' &&  //
          _token != '?' && _token != ':' && _token != '<') {
        // the key has been already consumed, parse a value from new line
        __retry(parse(&info_v));
      }
    } else if (_token != 'S' && !is_end(_token) &&  //
               _token != '?' && _token != ':' && _token != '<') {
      // the key has been already consumed, parse a value in the same line
      __retry(parse(&info_v));
    }

    map.insert(key, val_box.var());

    // add all other keys and values
    while (_token != 'S' && !is_end(_token) && _token != '<') {

      if (is_new_line(_token)) {
        next();
      }

      // get a key
      Box key_box(map.val_type());
      auto info_k = Reflection::reflect(key_box.var());

      if (_token == '$') {
        next();
        if (_token == ';') {
          next();
        }

        auto ex = info_k.match([this](Bool& b) -> Expected<None> { return b.set(parse_bool(string)); },
                               [this](Integer& i) -> Expected<None> { return i.set(parse_int(string)); },
                               [this](Float& f) -> Expected<None> { return f.set(parse_double(string)); },
                               [this](String& s) -> Expected<None> { return s.set(string); },
                               [this](Enum& e) -> Expected<None> { return e.parse(string); },
                               [this](auto&&) -> Expected<None> { return error_match(); });
        __retry(ex);

      } else if (_token == '?') {
        next();
        if (is_new_line(_token)) {
          next();
          if (_token == '-') {
            __retry(parse_seq(&info_k));
          } else if (_token == '>') {
            __retry(parse_ind(&info_k));
          }
        } else {
          matcher().insert_stop(matcher().columno());
          __retry(parse(&info_k));
          if (_token == '<') {
            next();
          }
        }
      } else {
        return error_token(_token);
      }

      // get a value
      if (_token == ':') {
        next();

        if (is_new_line(_token)) {
          next();
          if (_token == '-') {
            __retry(parse_seq(&info_v));
          } else if (_token == '>') {
            __retry(parse_ind(&info_v));
          }
        } else {
          matcher().insert_stop(matcher().columno());

          __retry(parse(&info_v));

          if (_token == '<') {
            next();
          }
        }
      } else {
        return error_token(_token);
      }

      map.insert(key_box.var(), val_box.var());
    }
    return None();
  }*/

  // parse "{ key:val, ... }"
  Expected<None> parse_flow_map(TypeInfo* info) {
    next();

    size_t level = 0;
    while (!is_end(_token) && _token != 'S' && _token != '}') {

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

      // get a key
      std::string key;
      if (_token == '$') {
        next();
        if (_token == ';') {
          next();
        }
        key = string;
      } else {
        return error("Only strings supported while parse a yaml map");
        /*
        if (_token == '[') {
          // parse_flow_seq(key);
        } else if (_token == '{') {
          // parse_flow_map(key);
        }
        */
      }

      // get a value
      if (_token == ':') {
        next();
        if (!is_new_line(_token) && _token != ',') {

          auto ex = info->get<Object>().get_field(key);
          __retry(ex);

          auto info_f = Reflection::reflect(ex.unwrap());
          __retry(parse(&info_f));
        }
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