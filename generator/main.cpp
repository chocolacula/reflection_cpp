#include <linux/limits.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"
#include "rr/reflection/printer.h"
#include "rr/serialization/yaml.h"
#include "rr/tools/format.h"
#include "self_generated/reflection.h"
#include "tclap/CmdLine.h"

// #include "parsing/enum.h"
// #include "parsing/struct.h"

std::filesystem::path current_dir() {
  char exe_path[PATH_MAX];
  ssize_t size = readlink("/proc/self/exe", exe_path, PATH_MAX);

  return std::filesystem::path(exe_path).remove_filename();
}

int main(int argc, const char** argv) {

  TCLAP::CmdLine cmd("Required Reflection code generator", ' ', VERSION);

  auto root = current_dir();

  root.append("config.yaml");
  TCLAP::ValueArg<std::string> c_arg("c", "config", "Explicitly specify path to the config file",  //
                                     false, root.string(), "path");
  root.remove_filename();

  cmd.add(c_arg);
  cmd.parse(argc, argv);

  std::string c_path = c_arg.getValue();

  std::ifstream input;
  input.open(c_path);

  if (!input.is_open()) {
    std::cerr << "Cannot find the config file, aborted" << std::endl;
    return -1;
  }

  auto conf = rr::serialization::json::from_stream<Config>(input).unwrap();

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
  return 0;
}