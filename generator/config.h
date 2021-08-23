#pragma once

#include <string>
#include <vector>

#include "rr/attributes.h"

RR_OBJECT()
struct Config {
  struct Templates {
    RR_ALIAS(enum)
    std::string for_enum;
    std::string object;
  } templates;

  std::string root_dir;
  std::vector<std::string> input;
  std::string output_dir;
};