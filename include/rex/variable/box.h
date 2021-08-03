#pragma once

#include "rex/reflection/reflection.h"
#include "var.h"

namespace rr {

struct Box {
  Box(const Box& other) = delete;
  Box(Box&& other) = default;

  explicit Box(TypeId id) : _id(id) {

    if (Reflection::copy_default(id, _data.stack_mem, kMemSize)) {
      _data.optimized = true;
    } else {
      _data.ptr = Reflection::alloc_default(id).value();
      _data.optimized = false;
    }
  }

  ~Box() {
    if (!_data.optimized) {
      Reflection::call_delete(Var(_data.ptr, _id));
    }
  }

  Box clone() {
    auto new_box = Box(_id);

    Reflection::copy(var(), new_box.var());

    return new_box;
  }

  Var var() {
    if (_data.optimized) {
      return Var(_data.stack_mem, _id);
    }

    return Var(_data.ptr, _id);
  }

 private:
  TypeId _id;

  static const size_t kMemSize = 39;  // alignment considering 1 byte for the optimization flag
  union Data {
    void* ptr;
    char stack_mem[kMemSize];
    bool optimized : kMemSize;
  } _data;
};

}  // namespace rr
