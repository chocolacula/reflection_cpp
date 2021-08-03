#pragma once

#include "../isequence.h"

namespace rr {

struct ISet : public ISequence {
  virtual Expected<None> remove(Var value) = 0;
  virtual bool contains(Var value) = 0;
};

}  // namespace rr