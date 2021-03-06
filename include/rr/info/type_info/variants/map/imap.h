#pragma once

#include <functional>

#include "../../../../variable/var.h"

namespace rr {

struct IMap {
  virtual Var own_var() const = 0;
  virtual TypeId key_type() const = 0;
  virtual TypeId val_type() const = 0;
  virtual void for_each(std::function<void(Var, Var)> callback) const = 0;
  virtual void for_each(std::function<void(Var, Var)> callback) = 0;
  virtual void clear() = 0;
  virtual size_t size() const = 0;
  virtual Expected<None> insert(Var key, Var value) = 0;
  virtual Expected<None> remove(Var key) = 0;
};

}  // namespace rr