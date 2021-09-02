#pragma once

#include <cstdint>

#include "../../../../expected.h"
#include "../../../../variable/var.h"

namespace rr {

struct IInteger {
  virtual Var var() = 0;
  virtual int64_t get() const = 0;
  virtual Expected<None> set(int64_t value) = 0;
  virtual std::string to_string() const = 0;
};

}  // namespace rr