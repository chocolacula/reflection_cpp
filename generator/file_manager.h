#pragma once

#include <linux/limits.h>
#include <unistd.h>

#include <array>
#include <fstream>
#include <string>
#include <string_view>

class FileManager {
 public:
  FileManager() {
    std::array<char, PATH_MAX> raw_path;

    auto size = readlink("/proc/self/exe", raw_path.data(), raw_path.size());
    _root = std::string(raw_path.data(), size);
    cut_filename(&_root);
  }

  const std::string& root() {
    return _root;
  }

  std::string correct_path(std::string_view path) {
    if (path[0] == '/') {
      return std::string(path);
    }

    auto t = _root;
    t += path;
    return t;
  }

  std::ofstream create_header(std::string_view dir) {
    std::ofstream header;
    header.open(correct_path(dir) + "/reflection.h");

    header << R"(#pragma once

#include "rr/reflection/reflection.h"
#include "rr/types/all_types.h"

// generated
)";

    return header;
  }

 private:
  std::string _root;

  static void cut_filename(std::string* str) {
    auto pos = str->find_last_of('/');

    if (pos != std::string::npos) {
      pos += 1;
      str->erase(pos, str->length() - pos);
    }
  }
};

std::string clear_name(const std::string& qualified_name) {
  auto pos = qualified_name.find_last_of(':');

  if (pos != std::string::npos) {
    pos += 1;
    return qualified_name.substr(pos, qualified_name.length() - pos);
  }

  return qualified_name;
}