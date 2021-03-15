#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>

#include "../error_parse.h"
#include "../lexers/compiled/cpp_source.yy.h"
#include "nlohmann/json.hpp"
#include "rex/expected/expected.h"
#include "states/enum.h"

// parses only one enum and ends execution
static Expected<nlohmann::json, ErrorParse> parse_enum(LexerCpp* lexer) {

  StatesEnum current_state = StatesEnum::kAmbiguous;

  nlohmann::json data;
  nlohmann::json constants;

  while (true) {

    auto token = static_cast<TokensCpp>(lexer->lex());

    switch (token) {
      case TokensCpp::kAttribute:
        if (lexer->is_equal("RR_NAME")) {

          if (current_state == StatesEnum::kAliasingEnd) {

            auto a = constants.size();
            constants.erase(constants.size() - 1);
          }

          current_state = StatesEnum::kAliasing;
        }
        break;

      case TokensCpp::kBraceL:

        if (current_state == StatesEnum::kBrace) {
          current_state = StatesEnum::kConstant;
        } else {
          return ErrorParse("Unexpected '{'", lexer->get_position());
        }
        break;

      case TokensCpp::kBraceR:

        if (current_state == StatesEnum::kConstant || current_state == StatesEnum::kAliasing) {
          current_state = StatesEnum::kEnd;
          data["constants"] = constants;

          goto break_while;
        } else {
          return ErrorParse("Unexpected '}'", lexer->get_position());
        }
        break;

      case TokensCpp::kWord:

        switch (current_state) {
          case StatesEnum::kAmbiguous:
            if (lexer->is_equal("enum")) {
              current_state = StatesEnum::kName;
            }
            break;

          case StatesEnum::kName:
            if (lexer->is_equal("class") == false) {
              data["name"] = lexer->text();
              current_state = StatesEnum::kBrace;
            }
            break;

          case StatesEnum::kConstant:
            constants.emplace_back(lexer->text());
            break;

          case StatesEnum::kAliasing:
            constants.emplace_back(lexer->text());
            current_state = StatesEnum::kAliasingEnd;
            break;

          case StatesEnum::kAliasingEnd:
            // skip constant and switch state in this case
            current_state = StatesEnum::kConstant;
            break;
        }
        break;

      default:
        goto break_while;
    }
  }

break_while:

  if (current_state == StatesEnum::kEnd)
    return data;

  return ErrorParse("Unexpected end of file while parsing process", lexer->get_position());
}