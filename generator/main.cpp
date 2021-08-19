#include <fstream>
#include <iostream>
#include <ostream>

#include "tclap/CmdLine.h"
#include "version.h"

// #include "parsing/enum.h"
// #include "parsing/struct.h"

int main(int argc, char** argv) {

  TCLAP::CmdLine cmd("Required Reflection generator", ' ', PROJECT_VER);

  TCLAP::ValueArg<std::string> c_arg("c", "config", "Explicitly specify path to the config file",  //
                                     false, "./config.yaml", "path");
  cmd.add(c_arg);
  cmd.parse(argc, argv);

  std::string c_path = c_arg.getValue();

  // Do what you intend.
  if (!c_path.empty()) {
    std::cout << c_path << std::endl;
  }

  std::ifstream input;
  input.open("../example/enum.h");

  // std::string word;
  // while (input >> word) {

  //   // this is the root tag and it has to be separate world
  //   if (word == "REX_ENUM") {

  //     auto ex = EnumParser::parse(&input);

  //     ex.match([](auto data) { std::cout << "found an enum with name " << data["name"] << std::endl; },
  //              [](Error err) { std::cout << err.what() << std::endl; });
  //   }
  //   // this is another one root tag and it has to be separate world
  //   else if (word == "REX_STRUCT") {

  //     parse_struct(&input);
  //   }
  // }
}