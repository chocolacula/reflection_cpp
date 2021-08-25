#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../../reflection/reflection.h"
#include "../../variable/box.h"
#include "lexers/compiled/lexer_json.yy.h"
#include "rr/info/type_info/variants/enum/enum.h"

namespace rr {

class ParserJson : public LexerJson {
 public:
  ParserJson(const char* input, size_t input_size) : LexerJson(reflex::Input(input, input_size)), _level(0) {
  }

  ParserJson(std::istream& stream) : LexerJson(stream), _level(0) {
  }

  void deserialize(TypeInfo* info) {
    parse(info);
  }

 private:
  enum Tag { UND, NUL, BOO, NUM, STR, ARR, OBJ, ERR } _tag;

  Tag parse(TypeInfo* info) {
    int token = lex();
    switch (token) {
      case '0':
        // do nothing
        return _tag = NUL;
      case 't':
        info->get<Bool>().set(true);
        return _tag = BOO;
      case 'f':
        info->get<Bool>().set(false);
        return _tag = BOO;
      case '#':
        info->match([this](Integer& i) { i.set(parseInt(string)); },   //
                    [this](Float& f) { f.set(parseDouble(string)); },  //
                    [](auto&&) {});
        return _tag = NUM;
      case '$':
        info->match([this](String& s) { s.set(string); },  //
                    [this](Enum& e) { e.parse(string); },  //
                    [](auto&&) {});
        return _tag = STR;
      case '[':
        return parse_array(info);
      case '{':
        return parse_object(info);
      default:
        return error(token);
    }
  }

  Tag parse_array(TypeInfo* info) {
    if (++_level > kMaxLevel) {
      return ERR;
    }

    info->match(
        [this](Array& a) {
          auto size = a.size() < kMaxsize ? a.size() : kMaxsize;
          auto type = a.nested_type();

          parse_items(size, type, [&a](size_t i, Var value) {
            auto item = a.at(i).unwrap();
            Reflection::copy(item, value);
          });
        },
        [this](Sequence& s) {
          auto type = s.nested_type();
          parse_items(kMaxsize, type, [&s](size_t, Var value) { s.push(value); });
        },
        [](auto&&) {});

    return ARR;
  }

  Expected<None> parse_items(size_t max, TypeId type, std::function<void(size_t, Var)> assign) {
    for (size_t len = 0; len < max; ++len) {

      Box box(type);
      auto boxed_info = Reflection::reflect(box.var());

      switch (parse(&boxed_info)) {
        case NUL:
          [[fallthrough]];
        case BOO:
          [[fallthrough]];
        case NUM:
          [[fallthrough]];
        case STR:
          [[fallthrough]];

        case ARR:
          [[fallthrough]];
        case OBJ:
          assign(len, box.var());
          break;

        case UND:
          --_level;
          if (len == 0) {
            _tag = ARR;
            return None();
          }
          return Error("");
        default:
          return Error("Got an Error");
      }

      int token = lex();
      if (token == ']') {
        --_level;
        _tag = ARR;
        return None();
      }
      if (token != ',') {
        return Error(format("Unexpected token '{}'", token));
      }
    }
    return None();
  }

  Tag parse_object(TypeInfo* info) {
    if (++_level > kMaxLevel) {
      return ERR;
    }

    for (size_t len = 0; len < kMaxsize; ++len) {
      int token = lex();

      if (len == 0 && token == '}') {
        --_level;
        return _tag = OBJ;
      }
      if (token != '$') {
        return ERR;
      }
      if (lex() != ':') {
        return ERR;
      }

      auto field = info->get<Object>().get_field(string).unwrap();
      auto field_info = Reflection::reflect(field);

      switch (parse(&field_info)) {
        case NUL:
          [[fallthrough]];
        case BOO:
          [[fallthrough]];
        case NUM:
          [[fallthrough]];
        case STR:
          [[fallthrough]];
        case ARR:
          [[fallthrough]];
        case OBJ:
          break;
        default:
          return ERR;
      }

      token = lex();
      if (token == '}') {
        --_level;
        return _tag = OBJ;
      }
      if (token != ',') {
        return ERR;
      }
    }
    return ERR;
  }

  Tag error(int token) {
    _tag = ERR;

    if (token == ']') {
      return _level > 0 ? UND : ERR;
    }
    return ERR;
  }

  static inline int64_t parseInt(const std::string& str) {
    return std::strtoll(&str[0], nullptr, 10);
  }

  static inline double parseDouble(const std::string& str) {
    return std::strtod(&str[0], nullptr);
  }

  static const size_t kMaxLevel = 100;  // to keep things secure: max JSON nesting depth
  static const size_t kMaxsize = 1000;  // to keep things secure: max JSON array and object size
  size_t _level;
};

}  // namespace rr