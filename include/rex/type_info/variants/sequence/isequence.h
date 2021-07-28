#pragma once

namespace rr {

struct ISequence {
  virtual Var var() = 0;
  virtual Expected<Var> first() = 0;
  virtual Expected<Var> last() = 0;
  virtual void for_each(std::function<void(Var)> callback) = 0;
  virtual void clear() = 0;
  virtual size_t size() = 0;
  virtual Expected<None> push(Var element) = 0;
};

}  // namespace rr