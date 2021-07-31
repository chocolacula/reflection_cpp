#pragma once

#include <vector>

#include "iarray.h"

namespace rr {

template <typename T, size_t size_v>
struct StdArray final : public IArray {

  explicit StdArray(std::array<T, size_v>* array) : _array(array) {
  }

  Var own_var() override {
    return Var(_array);
  }

  Expected<Var> at(size_t idx) override {
    if (idx >= size_v) {
      return Error(fmt::format("Index: {} is out of array's size: {}", idx, size_v));
    }

    return Var(&_array[idx]);
  }

  Expected<Var> operator[](size_t idx) override {
    return at(idx);
  }

  Expected<Var> first() override {
    return Var(_array);
  };

  Expected<Var> last() override {
    return Var(&_array[size_v - 1]);
  };

  Expected<None> fill(Var filler) override {
    auto f = filler.rt_cast<T>();

    return f.template match(
        [this](T* ptr) -> Expected<None> {  //
          _array->fill(*ptr);
          return None();
        },
        [](Error err) -> Expected<None> {  //
          return err;
        });
  }

  void for_each(std::function<void(Var)> callback) override {
    for (auto i = 0; i < size_v; ++i) {
      callback(Var(&(*_array)[i]));
    }
  }

  size_t size() override {
    return size_v;
  }

 private:
  std::array<T, size_v>* _array;
};

}  // namespace rr
