#pragma once

#include <string>
#include <vector>

#include "rex/attributes.h"

RR_OBJECT()
struct Config {
  struct Templates {
    std::string template_enum;
    std::string template_object;
  } templates;

  std::string root_dir;
  std::vector<std::string> input;
  std::string output_dir;
};