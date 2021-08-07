#pragma once

#include "rex/info/type_info/variants/sequence/list/list.h"
#include "rex/info/type_info/variants/sequence/queue/queue.h"
#include "rex/info/type_info/variants/sequence/set/set.h"
#include "rex/info/type_info/variants/sequence/stack/stack.h"
#include "rex/info/type_info/variants/sequence/vector/vector.h"

namespace rr {

#define BASE Variant<Vector, Stack, Queue, List, Set>

struct Sequence : public BASE, public ISequence {
  Sequence() = delete;

  template <typename T>
  Sequence(std::vector<T>* vector, bool is_const)  //
      : BASE(Vector(vector, is_const)) {
  }

  template <typename T>
  Sequence(std::list<T>* list, bool is_const)  //
      : BASE(List(list, is_const)) {
  }

  template <typename T>
  Sequence(std::deque<T>* deque, bool is_const)  //
      : BASE(List(deque, is_const)) {
  }

  template <typename T>
  Sequence(std::stack<T>* stack, bool is_const)  //
      : BASE(Stack(stack, is_const)) {
  }

  template <typename T>
  Sequence(std::queue<T>* queue, bool is_const)  //
      : BASE(Queue(queue, is_const)) {
  }

  template <typename T>
  Sequence(std::set<T>* set, bool is_const)  //
      : BASE(Set(set, is_const)) {
  }

  template <typename T>
  Sequence(std::unordered_set<T>* set, bool is_const)  //
      : BASE(Set(set, is_const)) {
  }

  Var own_var() const override {
    return match([](auto&& s) -> Var { return s.own_var(); });
  }

  void for_each(std::function<void(Var)> callback) const override {
    match([&](auto&& s) { s.for_each(callback); });
  }

  void clear() override {
    match([](auto&& s) { s.clear(); });
  }

  size_t size() override {
    return match([](auto&& s) -> size_t { return s.size(); });
  }

  Expected<None> push(Var value) override {
    return match([&](auto&& s) -> Expected<None> { return s.push(value); });
  }
};

#undef BASE

}  // namespace rr