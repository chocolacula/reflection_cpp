#pragma once

#include "../isequence.h"

namespace rr {

struct IStack : public ISequence {
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual void pop() = 0;
  virtual Expected<Var> top() = 0;
};

}  // namespace rr