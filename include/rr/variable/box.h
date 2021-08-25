#pragma once

#include "../reflection/reflection.h"

namespace rr {

struct Box {
  Box(const Box& other) = delete;
  Box(Box&& other) = default;

  explicit Box(TypeId id) : _type(id) {
    auto* ptr = Reflection::call_new(id, &_data.stack_mem[0], kMemSize);
    _optimized = (ptr == &_data.stack_mem[0]);
  }

  ~Box() {
    Reflection::call_delete(Var(&_data.stack_mem[0], _type, false), _optimized);
  }

  Var var() {
    if (_optimized) {
      return Var(&_data.stack_mem[0], _type, false);
    }
    return Var(_data.ptr, _type, false);
  }

  Box clone() {
    Box new_one(_type);
    Reflection::copy(new_one.var(), var());
    return new_one;
  }

 private:
  TypeId _type;
  bool _optimized;

  // max size of stack memory for dynamic allocation optimization
  static const size_t kMemSize = sizeof(std::unordered_map<int, int>);
  union Data {
    void* ptr;
    char stack_mem[kMemSize];
  } _data;
};

}  // namespace rr
