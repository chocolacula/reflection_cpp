#pragma once

#include <cstddef>

#include "enum.h"
#include "rex/attributes.h"

RR_OBJECT()
struct Bicycle {
  Bicycle() = default;

  int id;
  std::string manufacturer;
  std::string model;

  RR_ALIAS(weight)
  float frame_weight;

  RR_ALIAS(wheel_size)
  int wheel_size_inch;

  std::vector<Colors> colors;
};
