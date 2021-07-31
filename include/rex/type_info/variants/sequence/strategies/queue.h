#pragma once

#include <queue>

#include "../isequence.h"

namespace rr::strategy {

template <typename T>
struct Queue : public ISequence {

  explicit Queue(std::queue<T>* queue)
      : _queue(queue),  //
        _type(TypeId::get(queue)) {
  }

  Var var() override {
    return Var(_queue, _type);
  }

  Expected<Var> first() override {
    return Var(&_queue->front());
  };

  Expected<Var> last() override {
    return Var(&_queue->back());
  };

  void for_each(std::function<void(Var)> callback) override {
    for (auto&& entry : *_queue) {
      callback(Var(&entry));
    }
  }

  void clear() override {
    _queue->clear();
  }

  size_t size() override {
    return _queue->size();
  }

  Expected<None> push(Var element) override {

    _queue->push(*static_cast<T*>(element.raw()));
    return None();
  }

  Expected<None> pop() override {

    _queue->pop();
    return None();
  }

 private:
  std::queue<T>* _queue;
  TypeId _type;
};

}  // namespace rr::strategy
