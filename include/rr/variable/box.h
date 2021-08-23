#pragma once

#include "../reflection/reflection.h"

namespace rr {

struct Box {
  Box(const Box& other) = delete;
  Box(Box&& other) = default;

  explicit Box(TypeId id) : _id(id) {
    auto* ptr = Reflection::call_new(id, &_data.stack_mem[0], kMemSize);
    _optimized = (ptr == &_data.stack_mem[0]);
  }

  ~Box() {
    Reflection::call_delete(Var(&_data.stack_mem[0], _id, false), _optimized);
  }

  Var var() {
    if (_optimized) {
      return Var(&_data.stack_mem[0], _id, false);
    }
    return Var(_data.ptr, _id, false);
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
