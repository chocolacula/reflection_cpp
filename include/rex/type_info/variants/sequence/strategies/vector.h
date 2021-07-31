#pragma once

#include <vector>

#include "../isequence.h"

namespace rr::strategy {

template <typename T>
struct Vector : public ISequence {

  explicit Vector(std::vector<T>* vector)
      : _vector(vector),  //
        _type(TypeId::get(vector)) {
  }

  Var var() override {
    return Var(_vector, _type);
  }

  Expected<Var> first() override {
    return Var(&_vector->front());
  };

  Expected<Var> last() override {
    return Var(&_vector->back());
  };

  void for_each(std::function<void(Var)> callback) override {
    for (auto&& entry : *_vector) {
      callback(Var(&entry));
    }
  }

  void clear() override {
    _vector->clear();
  }

  size_t size() override {
    return _vector->size();
  }

  Expected<None> push(Var element) override {

    _vector->push_back(*static_cast<T*>(element.raw()));
    return None();
  }

  Expected<None> pop() override {

    _vector->pop_back();
    return None();
  }

 private:
  std::vector<T>* _vector;
  TypeId _type;
};

}  // namespace rr
