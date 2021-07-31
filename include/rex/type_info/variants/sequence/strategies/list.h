#pragma once

#include <list>

#include "../isequence.h"

namespace rr::strategy {

template <typename T>
struct List : public ISequence {

  explicit List(std::list<T>* list)
      : _list(list),  //
        _type(TypeId::get(list)) {
  }

  Var var() override {
    return Var(_list, _type);
  }

  Expected<Var> first() override {
    return Var(&_list->front());
  };

  Expected<Var> last() override {
    return Var(&_list->back());
  };

  void for_each(std::function<void(Var)> callback) override {
    for (auto&& entry : *_list) {
      callback(Var(&entry));
    }
  }

  void clear() override {
    _list->clear();
  }

  size_t size() override {
    return _list->size();
  }

  Expected<None> push(Var element) override {

    _list->push_back(*static_cast<T*>(element.raw()));
    return None();
  }

  Expected<None> pop() override {

    _list->pop_back();
    return None();
  }

 private:
  std::list<T>* _list;
  TypeId _type;
};

}  // namespace rr::strategy
