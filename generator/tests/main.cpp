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

  bool is_valid = data["name"] == "EnumClass1"        //
                  && data["constants"][0] == "White"  //
                  && data["constants"][1] == "Red"    //
                  && data["constants"][2] == "Green"  //
                  && data["constants"][3] == "Blue"   //
                  && data["constants"][4] == "Black";

  ASSERT_TRUE(is_valid);

  data = parse(&lexer).unwrap();
  is_valid = data["name"] == "EnumClass2"        //
             && data["constants"][0] == "White"  //
             && data["constants"][1] == "Red"    //
             && data["constants"][2] == "Green"  //
             && data["constants"][3] == "Blue"   //
             && data["constants"][4] == "Black";

  ASSERT_TRUE(is_valid);

  data = parse(&lexer).unwrap();

  is_valid = data["name"] == "EnumClass3"        //
             && data["constants"][0] == "White"  //
             && data["constants"][1] == "Red"    //
             && data["constants"][2] == "Green"  //
             && data["constants"][3] == "Blue";

  ASSERT_TRUE(is_valid);

  data = parse(&lexer).unwrap();
  is_valid = data["name"] == "EnumClass4"        //
             && data["constants"][0] == "white"  //
             && data["constants"][1] == "red"    //
             && data["constants"][2] == "green"  //
             && data["constants"][3] == "blue"   //
             && data["constants"][4] == "black";

  ASSERT_TRUE(is_valid);

  data = parse(&lexer).unwrap();
  is_valid = data["name"] == "EnumClass5"           //
             && data["constants"][0] == "white_2";  //

  ASSERT_TRUE(is_valid);

  data = parse(&lexer).unwrap();
  is_valid = data["name"] == "EnumClass6"  //
             && data["constants"].empty();

  ASSERT_TRUE(is_valid);

  data = parse(&lexer).unwrap();
  is_valid = data["name"] == "TheEnum"           //
             && data["constants"][0] == "White"  //
             && data["constants"][1] == "Red"    //
             && data["constants"][2] == "Green"  //
             && data["constants"][3] == "Blue"   //
             && data["constants"][4] == "Black";

  ASSERT_TRUE(is_valid);
}

TEST(Parse, Ini) {

  std::ifstream input;
  input.open("generator/tests/cases/configs/valid.ini");

  // CursorIni cursor_valid(input);

  // auto data = parse_ini(&cursor_valid).get();

  // ASSERT_EQ(data.template_enum, "enum2.inja");
  // ASSERT_EQ(data.template_object, "object.inja");
  // ASSERT_EQ(data.header_path, "../example/reflection.h");

  // input.open("generator/tests/cases/configs/shuffled.ini");
  // CursorIni cursor_shuddled(input);

  // data = parse_ini(&cursor_shuddled).get();

  // ASSERT_EQ(data.template_enum, "enum2.inja");
  // ASSERT_EQ(data.template_object, "object.inja");
  // ASSERT_EQ(data.header_path, "../example/reflection.h");

  // input.open("generator/tests/cases/configs/invalid.ini");
  // CursorIni cursor_invalid(input);

  // bool is_valid;

  // auto ex = parse_ini(&cursor_invalid);
  // ex.unbind_move([&is_valid](Config /*cfg*/) { is_valid = true; },  //
  //                [&is_valid](ErrorParse err) {
  //                  std::cout << err.what();
  //                  is_valid = false;
  //                });

  // ASSERT_FALSE(is_valid);
}
