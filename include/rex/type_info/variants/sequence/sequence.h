#pragma once

#include <vector>

#include "rex/reflection/type_actions.h"
#include "rex/type_info/variants/array/array.h"
#include "strategies/deque.h"
#include "strategies/list.h"
#include "strategies/queue.h"
#include "strategies/stack.h"
#include "strategies/vector.h"

namespace rr {

struct Sequence : public ISequence {

  Sequence() = delete;

  template <template <typename T1> typename ContainerT, typename T>
  explicit Sequence(ContainerT<T>* container)  //
      : _sequence(choose_strategy(container)), _nested_type(TypeId::get<T>()) {
  }

  Var var() override {
    return _sequence->var();
  }

  Expected<Var> first() override {
    if (_sequence->size() == 0) {
      return Error("The sequence is empty");
    }

    return _sequence->first();
  };

  Expected<Var> last() override {
    if (_sequence->size() == 0) {
      return Error("The sequence is empty");
    }

    return _sequence->last();
  };

  void for_each(std::function<void(Var)> callback) override {
    _sequence->for_each(callback);
  }

  void clear() override {
    _sequence->clear();
  }

  size_t size() override {
    return _sequence->size();
  }

  Expected<None> push(Var element) override {
    if (_nested_type != element.type()) {
      return Error(fmt::format("Trying to push value({}) to sequence<{}>",  //
                               TypeActions::type_name(element.type()),      //
                               TypeActions::type_name(_nested_type)));
    }

    return _sequence->push(element);
  }

  Expected<None> pop() override {

    if (_sequence->size() == 0) {
      return Error("The sequence is empty");
    }

    return _sequence->pop();
  }

 private:
  std::shared_ptr<ISequence> _sequence;
  TypeId _nested_type;

  template <typename T>
  std::shared_ptr<ISequence> choose_strategy(std::vector<T>* vector) {
    return std::make_shared<strategy::Vector<T>>(vector);
  }

  template <typename T>
  std::shared_ptr<ISequence> choose_strategy(std::list<T>* list) {
    return std::make_shared<strategy::List<T>>(list);
  }

  template <typename T>
  std::shared_ptr<ISequence> choose_strategy(std::stack<T>* stack) {
    return std::make_shared<strategy::Stack<T>>(stack);
  }

  template <typename T>
  std::shared_ptr<ISequence> choose_strategy(std::queue<T>* queue) {
    return std::make_shared<strategy::Queue<T>>(queue);
  }

  template <typename T>
  std::shared_ptr<ISequence> choose_strategy(std::deque<T>* deque) {
    return std::make_shared<strategy::Deque<T>>(deque);
  }
};

}  // namespace rr