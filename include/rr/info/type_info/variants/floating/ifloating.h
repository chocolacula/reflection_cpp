#pragma once

#include <cfloat>
#include <iomanip>
#include <sstream>

#include "../../../../expected.h"
#include "../../../../variable/var.h"

namespace rr {

struct IFloating {
  virtual Var var() = 0;
  virtual double get() = 0;
  virtual Expected<None> set(double value) = 0;
  virtual std::string to_string(int precision) const = 0;
};

}  // namespace rr