#pragma once

#include <vector>

#include "../isequence.h"

namespace rr {

template <typename T, size_t size_v>
struct ArrayStrategy : public ISequence {

  explicit ArrayStrategy(T (*array)[size_v])
      : _array(reinterpret_cast<T*>(array)),  //
        _type(TypeId::get(array)) {
  }

  explicit ArrayStrategy(std::array<T, size_v>* array)
      : _array(array->data()),  //
        _type(TypeId::get(array)) {
  }

  Var var() override {
    return Var(_array, _type);
  }

  Expected<Var> first() override {
    return Var(_array);
  };

  Expected<Var> last() override {
    return Var(&_array[size_v - 1]);
  };

  void for_each(std::function<void(Var)> callback) override {
    for (auto i = 0; i < size_v; ++i) {
      callback(Var(&_array[i]));
    }
  }

  void clear() override {
    std::memset(_array, 0, size_v * sizeof(T));
    _idx = 0;
  }

  size_t size() override {
    return size_v;
  }

  Expected<None> push(Var element) override {

    if (_idx == size_v) {
      return Error("The sequence is full");
    }

    _array[_idx++] = *static_cast<T*>(element.value());
    return None();
  }

 private:
  T* _array;
  size_t _idx = 0;

  TypeId _type;
};

}  // namespace rr
