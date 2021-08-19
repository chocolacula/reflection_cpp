#pragma once

#include "../reflection/reflection.h"

namespace rr {

struct Box {
  Box(const Box& other) = delete;
  Box(Box&& other) = default;

  explicit Box(TypeId id) : _id(id) {
    if (Reflection::copy_default(id, &_data.stack_mem[0], kMemSize)) {
      _optimized = true;
    } else {
      _data.ptr = Reflection::alloc_default(id).raw_mut();
      _optimized = false;
    }
  }

  ~Box() {
    if (!_optimized) {
      Reflection::call_delete(Var(_data.ptr, _id, false));
    }
  }

  Box clone() {
    auto new_box = Box(_id);
    Reflection::copy(new_box.var(), var());
    return new_box;
  }

  Var var() {
    if (_optimized) {
      return Var(&_data.stack_mem[0], _id, false);
    }
    return Var(_data.ptr, _id, false);
  }

  static size_t s() {
    return sizeof(_data);
  }

 private:
  TypeId _id;
  bool _optimized;

  // max size of stack memory for dynamic allocation optimization
  static const size_t kMemSize = sizeof(std::unordered_map<int, int>);
  union Data {
    void* ptr;
    char stack_mem[kMemSize];
  } _data;
};

}  // namespace rr
