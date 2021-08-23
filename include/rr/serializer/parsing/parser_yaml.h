#pragma once

#include <string>

#include "../../reflection/reflection.h"
#include "../../variable/box.h"
#include "lexers/compiled/lexer_yaml.yy.h"

class ParserYaml : public LexerYaml {

 public:
  ParserYaml(const char* input, size_t input_size) : LexerYaml(reflex::Input(input, input_size)), _token(lex()) {
  }

  ParserYaml(std::istream& stream) : LexerYaml(stream), _token(lex()) {
  }

  void deserialize(rr::TypeInfo* info) {
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
  void parse(rr::TypeInfo* info) {
    if (is_new_line(_token))
      next();

    if (_token == '!') {
      _tag = string;
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
  /*
    void parse_by_type(rr::TypeInfo* info) {
      info->match(
          [this](rr::Object& o) {
            next_scalar();
            auto field_name = _lexer.string;
            auto field_info = rr::Reflection::reflect(o.get_field(field_name).unwrap());
            parse(&field_info);
          },                       //
          [](rr::Array& a) {},     //
          [](rr::Sequence& s) {},  //
          [](rr::Map& m) {},       //
          [this, info](auto&& other) { parse_scalar(info); });
    }

    void parse_scalar(rr::TypeInfo* info) {
      info->match(
          [this](rr::Bool& b) {
            std::transform(_lexer.string.begin(), _lexer.string.end(), _lexer.string.begin(), std::towlower);

            if (_lexer.string == "false" ||  //
                _lexer.string == "n" ||      //
                _lexer.string == "no" ||     //
                _lexer.string == "off") {
              b.set(false);
            } else {
              b.set(true);
            }
          },
          [this](rr::Integer& i) { i.set(std::stoi(_lexer.string)); },  //
          [this](rr::Float& f) { f.set(std::stod(_lexer.string)); },    //
          [this](rr::String& s) { s.set(_lexer.string); },              //
          [this](rr::Enum& e) { e.parse(_lexer.string); },              //
          [this](auto&&) {  });

      next();
    }
  */
  // parse "? key : val ..."
  void parse_key(rr::TypeInfo* info) {
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
  void parse_seq(rr::TypeInfo* info) {
    // if (_tag.empty()) {
    //   _tag = "!!seq";
    // }
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
      auto nested_type = info->get<rr::Sequence>().nested_type();
      rr::Box box(nested_type);

      auto new_info = rr::Reflection::reflect(box.var());
      parse(&new_info);

      info->get<rr::Sequence>().push(box.var());
    }
  }

  // parse indented value (string, nested sequence, or nested map)
  void parse_ind(rr::TypeInfo* info) {
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
  void parse_str() {  // rr::TypeInfo* info) {
    /*if (_tag.empty()) {
      _tag = "!!str";
    }

    info->match(
        [this](rr::Bool& b) {
          std::transform(string.begin(), string.end(), string.begin(), std::towlower);
          if (string == "false" || string == "no" || string == "n" || string == "off") {
            b.set(false);
          } else {
            b.set(true);
          }
        },
        [this](rr::Integer& i) { i.set(std::stoi(string)); },  //
        [this](rr::Float& f) { f.set(std::stod(string)); },    //
        [this](rr::String& s) { s.set(string); },              //
        [this](rr::Enum& e) { e.parse(string); },              //
        [this](auto&&) {});*/

    next();
    if (_token == ';') {
      next();
    }
  }

  // parse string of "key: val ..."
  void parse_str_or_map(rr::TypeInfo* info) {
    // if (_tag.empty())
    // _tag = "!!str";
    // data.str = string;
    next();

    if (_token == ':') {
      // it is a map
      parse_map(info);
    } else if (is_new_line(_token) || is_end(_token)) {
      info->get<rr::String>().set(string);
      next();
    }
  }

  // key is given in data, now parse ": val ..."
  void parse_map(rr::TypeInfo* info) {
    auto field_var = info->get<rr::Object>().get_field(string).unwrap();
    auto field_info = rr::Reflection::reflect(field_var);

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
    // _tag = "!!map";

    // add all other keys and values
    while (_token != 'S' && _token != 0 && _token != 'E' && _token != '<') {
      // YAML key, val;
      std::string key;

      if (_token == ';' || _token == '=') {
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
            field_var = info->get<rr::Object>().get_field(key).unwrap();
            auto s_info = rr::Reflection::reflect(field_var);

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

      if (_token == ':') {
        field_var = info->get<rr::Object>().get_field(key).unwrap();
        auto f_info = rr::Reflection::reflect(field_var);

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
      // data.map.push_back(YAML::Duo(key, val));
    }
  }

  // parse "[ val, ... ]"
  void parse_flow_seq(rr::TypeInfo* info) {
    if (_tag.empty()) {
      _tag = "!!seq";
    }
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
  void parse_flow_map(rr::TypeInfo* info) {
    if (_tag.empty()) {
      _tag = "!!map";
    }
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

  void next_scalar() {
    while (!is_end(_token) || _token != '$') {
      _token = lex();
    }
  }

  static inline bool is_end(int token) {
    return token == 0 || token == 'E';
  }

  static inline bool is_new_line(int token) {
    return token == '=' || token == ';';
  }

  int _token;
  std::string _tag;

  std::unordered_map<std::string, rr::Var> _anchors;
};
