#pragma once

#include <string>
#include <vector>

#include "rr/attributes.h"

ER_REFLECT()
struct Config {
  std::string root_dir;
  std::string compdb_dir;

  struct Templates {
    ER_ALIAS("enum")
    std::string for_enum;
    std::string object;
  } templates;

  std::vector<std::string> input;
  std::string output_dir;
};