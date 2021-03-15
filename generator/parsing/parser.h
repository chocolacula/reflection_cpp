#pragma once

#include <iostream>

#include "error_parse.h"
#include "nlohmann/json.hpp"
#include "parsers/enum.h"
#include "parsers/object.h"
#include "tokens/cpp_source.h"

static Expected<nlohmann::json, ErrorParse> parse(LexerCpp* lexer) {

  while (true) {

    auto token = static_cast<TokensCpp>(lexer->lex());

    if (token == TokensCpp::kAttribute || token == TokensCpp::kEof)
      break;
  }

  if (lexer->is_equal("RR_OBJECT")) {

    return parse_object(lexer);
  }

  if (lexer->is_equal("RR_ENUM")) {

    return parse_enum(lexer);
  }

  return ErrorParse("Nothing found to parse", lexer->get_position());
}
