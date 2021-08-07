#pragma once

#include <queue>

#include "iqueue.h"

namespace rr {

template <typename T>
struct StdQueue : public IQueue {
  StdQueue() = delete;

  explicit StdQueue(std::queue<T>* queue, bool is_const)
      : _queue(queue),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_queue, TypeId::get(_queue), _is_const);
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_queue) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_queue) {
      callback(Var(&entry, nested_type, _is_const));
    }
  }

  void clear() override {
    _queue->clear();
  }

  size_t size() override {
    return _queue->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return Error(format("Trying to set with type: {} to queue<{}>",  //
                          Reflection::type_name(value.type()),         //
                          Reflection::type_name(nested_type)));
    }
    _queue->push(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _queue->pop();
  }

  Expected<Var> front() override {
    return Var(&_queue->front());
  };

  Expected<Var> back() override {
    return Var(&_queue->back());
  };

 private:
  std::queue<T>* _queue;
  bool _is_const;
};

}  // namespace rr
