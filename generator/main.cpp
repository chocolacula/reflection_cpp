#include <linux/limits.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "clang/Tooling/CompilationDatabase.h"
#include "config.h"
#include "parsing/parser_cpp.h"
#include "rr/serialization/yaml.h"
#include "self_generated/reflection.h"
#include "tclap/CmdLine.h"

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

  TCLAP::ValueArg<int> j_arg("j", "jobs", "Allow N jobs at once, takes CPU core number by default",  //
                             false, std::thread::hardware_concurrency(), "N");

  cmd.add(c_arg);
  cmd.add(j_arg);
  cmd.parse(argc, argv);

  std::string c_path = c_arg.getValue();

  std::ifstream input;
  input.open(c_path);

  if (!input.is_open()) {
    std::cerr << "Cannot find the config file, aborted" << std::endl;
    return -1;
  }

  auto conf = rr::serialization::yaml::from_stream<Config>(input).unwrap();
  input.close();

  std::filesystem::path working_dir;

  if (conf.root_dir[0] == '/') {
    working_dir.assign(conf.root_dir);
  } else {
    working_dir = root;
    working_dir.append(conf.root_dir);
  }

  std::string err;
  std::string compdb_file = working_dir.string() + "/" + conf.compdb_dir;
  auto compdb = CompilationDatabase::autoDetectFromDirectory(compdb_file, err);

  for (auto&& file_name : conf.input) {

    auto file_path = working_dir;
    file_path.append(file_name);

    ParserCpp parser(compdb.get(), std::move(file_path.string()));
    parser.get_one();
  }

  // std::unordered_map<std::string, Node> registry;
  // take one from input
  // process it multithreaded
  // put Node to the registry

  // convert Node to nlohman::json, pay attention to namespaces and parent classes
  // generate file

  return 0;
}