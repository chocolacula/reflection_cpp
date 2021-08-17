#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../parsing/lexers/compiled/cpp_source.yy.h"
#include "../parsing/parser.h"
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

TEST(ParseTest, Enum) {

  std::ifstream input;

  input.open(SOURCE_DIR "/cases/enums.h");

  LexerCpp lexer(input);

  auto data = parse(&lexer).unwrap();
}

TEST(Parse, Ini) {

  std::ifstream input;
  input.open("generator/tests/cases/configs/valid.ini");
}
