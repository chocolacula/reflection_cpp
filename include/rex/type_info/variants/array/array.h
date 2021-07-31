#pragma once

#include <vector>

#include "c_array.h"
#include "iarray.h"
#include "std_array.h"

namespace rr {

struct Array final : public IArray {

  Array() = delete;

  template <typename T, size_t size_v>
  explicit Array(T (*array)[size_v]) : _array(std::make_shared<CArray<T, size_v>>(array)) {
  }

  template <typename T, size_t size_v>
  explicit Array(std::array<T, size_v>* array) : _array(std::make_shared<StdArray<T, size_v>>(array)) {
  }

  Var own_var() override {
    return _array->own_var();
  }

  Expected<Var> at(size_t idx) override {
    return _array->at(idx);
  }

  Expected<Var> operator[](size_t idx) override {
    return _array->operator[](idx);
  }

  Expected<Var> first() override {
    return _array->first();
  };

  Expected<Var> last() override {
    return _array->last();
  };

  Expected<None> fill(Var filler) override {
    return _array->fill(filler);
  }

  void for_each(std::function<void(Var)> callback) override {
    _array->for_each(callback);
  }

  size_t size() override {
    return _array->size();
  }

 private:
  std::shared_ptr<IArray> _array;
};

}  // namespace rr
