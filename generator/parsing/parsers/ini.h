#pragma once

#include <string_view>

#include "../../config.h"
#include "../error_parse.h"
#include "rex/expected/expected.h"

static Expected<Config, ErrorParse> parse_ini(LexerCpp* lexer) {
}