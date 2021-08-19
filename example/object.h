#pragma once

#include <cstddef>

#include "enum.h"
#include "rr/attributes.h"

RR_OBJECT()
struct Bicycle {
  Bicycle() = default;

  int id;
  bool is_hardtail = true;
  std::string manufacturer;
  std::string model;

  RR_ALIAS(weight)
  float frame_weight;

  RR_ALIAS(wheel_size)
  float wheel_size_inch;

  std::vector<Colors> colors;
};
