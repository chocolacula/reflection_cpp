#pragma once

#include <stack>

#include "istack.h"

namespace rr {

template <typename T>
struct StdStack : public IStack {
  StdStack() = delete;

  explicit StdStack(std::stack<T>* stack, bool is_const)
      : _stack(stack),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_stack, TypeId::get(_stack), _is_const);
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_stack) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_stack) {
      callback(Var(&entry, nested_type, _is_const));
    }
  }

  void clear() override {
    _stack->clear();
  }

  size_t size() override {
    return _stack->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return Error(format("Trying to set with type: {} to set<{}>",  //
                          Reflection::type_name(value.type()),       //
                          Reflection::type_name(nested_type)));
    }
    _stack->insert(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _stack->pop();
  }

  Expected<Var> top() override {
    if (_stack->empty()) {
      return Error("The stack is empty");
    }

    return Var(&_stack->top(), TypeId::get<T>(), _is_const);
  };

 private:
  std::stack<T>* _stack;
  bool _is_const;
};

}  // namespace rr
