#include <fstream>
#include <iostream>
#include <ostream>

#include "parsing/enum.h"
#include "parsing/struct.h"

int main() {
  std::ifstream input;
  input.open("../example/enum.h");

  // read fucking text
  // generate file from template by using parsed data

  std::string word;
  while (input >> word) {

    // this is the root tag and it has to be separate world
    if (word == "REX_ENUM") {

      auto ex = EnumParser::parse(&input);

      ex.match([](auto data) { std::cout << "found an enum with name " << data["name"] << std::endl; },
               [](Error err) { std::cout << err.what() << std::endl; });
    }
    // this is another one root tag and it has to be separate world
    else if (word == "REX_STRUCT") {

      parse_struct(&input);
    }
  }
}