#pragma once

#include "../../../var/var.h"

namespace rr {

struct IMap {
  virtual Var own_var() = 0;
  virtual void for_each(std::function<void(Var, Var)> callback) = 0;
  virtual void clear() = 0;
  virtual size_t size() = 0;
  virtual Expected<None> insert(Var key, Var value) = 0;
  virtual Expected<None> remove(Var key) = 0;
};

}  // namespace rr