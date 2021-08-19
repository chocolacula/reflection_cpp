#pragma once

#include <cstddef>

#include "rr/attributes.h"

RR_OBJECT()
struct Bicycle {
  Bicycle() = default;

  std::string manufacturer;
  std::string model;

  float weight;

  RR_ALIAS(size)
  char frame_size;

  std::vector<std::vector<int>> error_codes = {{400, 404, 502}, {600, 601}};

  RR_EXCLUDE
  float timeout = 6.5;
};
