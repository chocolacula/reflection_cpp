#pragma once

#include <deque>

#include "../isequence.h"

namespace rr::strategy {

template <typename T>
struct Deque : public ISequence {

  explicit Deque(std::deque<T>* deque)
      : _deque(deque),  //
        _type(TypeId::get(deque)) {
  }

  Var var() override {
    return Var(_deque, _type);
  }

  Expected<Var> first() override {
    return Var(&_deque->front());
  };

  Expected<Var> last() override {
    return Var(&_deque->back());
  };

  void for_each(std::function<void(Var)> callback) override {
    for (auto&& entry : *_deque) {
      callback(Var(&entry));
    }
  }

  void clear() override {
    _deque->clear();
  }

  size_t size() override {
    return _deque->size();
  }

  Expected<None> push(Var element) override {

    _deque->push_back(*static_cast<T*>(element.raw()));
    return None();
  }

  Expected<None> pop() override {

    _deque->pop_back();
    return None();
  }

 private:
  std::deque<T>* _deque;
  TypeId _type;
};

}  // namespace rr::strategy
