#pragma once

#include <functional>

#include "../../../var/var.h"

namespace rr {

struct IArray {

  virtual Var own_var() = 0;
  virtual Expected<Var> at(size_t idx) = 0;
  virtual Expected<Var> operator[](size_t idx) = 0;
  virtual Expected<Var> first() = 0;
  virtual Expected<Var> last() = 0;
  virtual Expected<None> fill(Var filler) = 0;
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual size_t size() = 0;
};

}  // namespace rr
