#pragma once

#include <string>

#include "../../reflection/reflection.h"
#include "../../variable/box.h"
#include "lexers/compiled/lexer_yaml.yy.h"

namespace rr {

class ParserYaml : public LexerYaml {
 public:
  ParserYaml(const char* input, size_t input_size) : LexerYaml(reflex::Input(input, input_size)), _token(lex()) {
  }

  ParserYaml(std::istream& stream) : LexerYaml(stream), _token(lex()) {
  }

  void deserialize(TypeInfo* info) {
    while (true) {

      if (_token == 'S') {
        next();
      } else if (is_end(_token)) {
        // break in the end of one document even if there are few in a file
        break;
      }
      parse(info);
    }
  }

 private:
  void parse(TypeInfo* info) {
    if (is_new_line(_token))
      next();

    // anchors support
    if (_token == '&') {
      // TODO get name of anchor
      next();
    } else if (_token == '*') {
      // TODO find a var with the same name and copy it
      next();
    }

    // parse_by_type(info);

    switch (_token) {
      case '-':
        parse_seq(info);
        break;
      case '>':
        parse_ind(info);
        break;
      case '$':
        parse_str_or_map(info);
        break;
      case '[':
        parse_flow_seq(info);
        break;
      case '{':
        parse_flow_map(info);
        break;
      case '?':
        parse_key(info);
        break;
      case ':':
        parse_map(info);
        break;
      default:
        next();
        break;
    }
  }

  // parse "? key : val ..."
  void parse_key(TypeInfo* info) {
    next();
    if (_token == ';' || _token == '=') {
      next();
      if (_token == '>')
        parse_ind(info);
    } else {
      matcher().insert_stop(matcher().columno());
      parse(info);
      if (_token == '<')
        next();
    }
    if (_token == ':')
      parse_map(info);
  }

  // parse "- val ..."
  void parse_seq(TypeInfo* info) {
    size_t level = 0;
    while (true) {
      if (_token == '>') {
        next();
        ++level;
      }
      while (_token == '<') {
        if (level == 0) {
          return;
        }
        next();
        if (level == 1) {
          return;
        }
        --level;
      }
      if (_token != '-') {
        break;
      }
      next();

      // TODO handle Array too
      auto nested_type = info->get<Sequence>().nested_type();
      Box box(nested_type);

      auto new_info = Reflection::reflect(box.var());
      parse(&new_info);

      info->get<Sequence>().push(box.var());
    }
  }

  // parse indented value (string, nested sequence, or nested map)
  void parse_ind(TypeInfo* info) {
    next();
    if (_token == '-') {
      parse_seq(info);
    } else {
      bool sp = true;
      size_t level = 0;

      std::string full_string;
      while (_token == '$') {
        if (full_string.empty()) {
          full_string = string;
        } else if (sp) {
          full_string.append(" ").append(string);
        } else {
          full_string.append(string);
        }
        sp = true;
        next();
        if (_token == ';') {
          next();
        } else if (_token == '=') {
          full_string.append(newlines() - 1, '\n');
          sp = false;
          next();
        } else {
          break;
        }
        if (_token == '>') {
          ++level;
          full_string.push_back('\n');
          sp = false;
          next();
        }
        while (_token == '<') {
          if (level == 0) {
            break;
          }
          next();
          --level;
        }
      }
      string = full_string;
    }

    if (_token == ':') {
      parse_map(info);
    }
    if (_token == '<') {
      next();
    }
  }

  // parse string
  void parse_str() {  // TypeInfo* info) {
    /*
    info->match(
        [this](Bool& b) {
          std::transform(string.begin(), string.end(), string.begin(), std::towlower);
          if (string == "false" || string == "no" || string == "n" || string == "off") {
            b.set(false);
          } else {
            b.set(true);
          }
        },
        [this](Integer& i) { i.set(std::stoi(string)); },  //
        [this](Float& f) { f.set(std::stod(string)); },    //
        [this](String& s) { s.set(string); },              //
        [this](Enum& e) { e.parse(string); },              //
        [this](auto&&) {});*/

    next();
    if (_token == ';') {
      next();
    }
  }

  // parse string of "key: val ..."
  void parse_str_or_map(TypeInfo* info) {
    next();

    if (_token == ':') {
      // it is a map
      parse_map(info);
    } else if (is_new_line(_token) || is_end(_token)) {
      info->get<String>().set(string);
      next();
    }
  }

  // key is given in data, now parse ": val ..."
  void parse_map(TypeInfo* info) {
    auto field_var = info->get<Object>().get_field(string).unwrap();
    auto field_info = Reflection::reflect(field_var);

    next();

    if (is_new_line(_token)) {
      next();
      if (_token != 'S' && _token != 0 && _token != 'E' && _token != '$' &&  //
          _token != '?' && _token != ':' && _token != '<') {

        parse(&field_info);
      }
    } else if (_token != 'S' && _token != 0 && _token != 'E' &&  //
               _token != '?' && _token != ':' && _token != '<') {

      parse(&field_info);
    }

    // add all other keys and values
    while (_token != 'S' && _token != 0 && _token != 'E' && _token != '<') {
      // YAML key, val;
      std::string key;

      if (is_new_line(_token)) {
        next();
      }

      // get key
      if (_token == '$') {
        parse_str();
        key = string;
      } else if (_token == '?') {
        next();
        if (_token == ';' || _token == '=') {
          next();
          if (_token == '-') {

            key = string;
            field_var = info->get<Object>().get_field(key).unwrap();
            auto s_info = Reflection::reflect(field_var);

            parse_seq(&s_info);
          } else if (_token == '>') {
            // parse_ind(key);
          }
        } else {
          matcher().insert_stop(matcher().columno());
          // parse(key);
          if (_token == '<')
            next();
        }
      }

      // get value
      if (_token == ':') {
        field_var = info->get<Object>().get_field(key).unwrap();
        auto f_info = Reflection::reflect(field_var);

        next();
        if (_token == ';' || _token == '=') {
          next();
          if (_token == '-') {
            parse_seq(&f_info);
          } else if (_token == '>') {
            parse_ind(&f_info);
          }
        } else {
          matcher().insert_stop(matcher().columno());
          parse(&f_info);
          if (_token == '<') {
            next();
          }
        }
      } else {
        break;
      }
    }
  }

  // parse "[ val, ... ]"
  void parse_flow_seq(TypeInfo* info) {
    size_t level = 0;
    next();
    while (_token != 0 && _token != 'S' && _token != 'E' && _token != ']') {
      // YAML val;
      if (_token == ';' || _token == '=') {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0)
            --level;
          next();
        }
      }
      // parse(val);
      // data.seq.push_back(val);
      if (_token == ';' || _token == '=') {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0)
            --level;
          next();
        }
      }
      if (_token == ',')
        next();
      if (_token == ';' || _token == '=')
        next();
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0)
            --level;
          next();
        }
      }
    }
    if (_token == ']') {
      next();
    }
    if (_token == ';' || _token == '=') {
      next();
    }
    while (_token == '<' && level-- > 0) {
      next();
    }
  }

  // parse "{ key:val, ... }"
  void parse_flow_map(TypeInfo* info) {
    size_t level = 0;
    next();
    while (_token != 0 && _token != 'S' && _token != 'E' && _token != '}') {
      // YAML key, val;
      if (_token == ';' || _token == '=') {
        next();
      }
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0)
            --level;
          next();
        }
      }
      if (_token == '$') {
        // parse_str(key);
      } else if (_token == '[') {
        // parse_flow_seq(key);
      } else if (_token == '{') {
        // parse_flow_map(key);
      }
      if (_token == ':') {
        next();
        if (_token != ';' && _token != '=' && _token != ',') {
          // parse(val);
        }
      }
      // data.map.push_back(YAML::Duo(key, val));
      if (_token == ';' || _token == '=')
        next();
      if (_token == '>') {
        ++level;
        next();
      } else {
        while (_token == '<') {
          if (level > 0)
            --level;
          next();
        }
      }
      if (_token == ',')
        next();
      if (_token == ';' || _token == '=')
        next();
      if (_token == '>') {
        ++level;
        next();
      } else if (_token == '<') {
        if (level > 0)
          --level;
        next();
      }
    }
    if (_token == '}')
      next();
    if (_token == ';' || _token == '=')
      next();
    while (_token == '<' && level-- > 0) next();
  }

  int next() {
    if (_token != 0) {
      _token = lex();
    }
    return _token;
  }

  static inline bool is_end(int token) {
    return token == 0 || token == 'E';
  }

  static inline bool is_new_line(int token) {
    return token == '=' || token == ';';
  }

  int _token;
  std::unordered_map<std::string, Var> _anchors;
};

}  // namespace rr