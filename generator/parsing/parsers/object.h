#pragma once

#include <iostream>

#include "../error_parse.h"
#include "../lexers/compiled/cpp_source.yy.h"
#include "nlohmann/json.hpp"
#include "rex/expected.h"

Expected<nlohmann::json, ErrorParse> parse_object(LexerCpp* lexer) {

  return ErrorParse("ololo", lexer->get_position());
}
