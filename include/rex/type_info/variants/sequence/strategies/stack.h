#pragma once

#include <stack>

#include "../isequence.h"

namespace rr::strategy {

template <typename T>
struct Stack : public ISequence {

  explicit Stack(std::stack<T>* stack)
      : _stack(stack),  //
        _type(TypeId::get(stack)) {
  }

  Var var() override {
    return Var(_stack, _type);
  }

  Expected<Var> first() override {
    return Var(&_stack->front());
  };

  Expected<Var> last() override {
    return Var(&_stack->back());
  };

  void for_each(std::function<void(Var)> callback) override {
    for (auto&& entry : *_stack) {
      callback(Var(&entry));
    }
  }

  void clear() override {
    _stack->clear();
  }

  size_t size() override {
    return _stack->size();
  }

  Expected<None> push(Var element) override {

    _stack->push(*static_cast<T*>(element.raw()));
    return None();
  }

  Expected<None> pop() override {

    _stack->pop();
    return None();
  }

 private:
  std::stack<T>* _stack;
  TypeId _type;
};

}  // namespace rr::strategy
