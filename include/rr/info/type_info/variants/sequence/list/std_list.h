#pragma once

#include <list>

#include "ilist.h"

namespace rr {

template <typename T>
struct StdList : public IList {
  StdList() = delete;

  explicit StdList(std::list<T>* list, bool is_const)
      : _list(list),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_list, TypeId::get(_list), _is_const);
  }

  void for_each(std::function<void(Var)> callback) const override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_list) {
      callback(Var(&entry, nested_type, true));
    }
  }

  void for_each(std::function<void(Var)> callback) override {
    auto nested_type = TypeId::get<T>();

    for (auto&& entry : *_list) {
      callback(Var(&entry, nested_type, _is_const));
    }
  }

  void clear() override {
    _list->clear();
  }

  size_t size() override {
    return _list->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return Error(format("Trying to set with type: {} to list<{}>",  //
                          Reflection::type_name(value.type()),        //
                          Reflection::type_name(nested_type)));
    }
    _list->push_back(*static_cast<const T*>(value.raw()));
    return None();
  }

  void pop() override {
    _list->pop_back();
  }

  void push_front(Var element) override {
    _list->push_front(*static_cast<const T*>(element.raw()));
  }

  void pop_front() override {
    _list->pop_front();
  }

  Expected<Var> front() override {
    if (_list->empty()) {
      return Error("The list is empty");
    }
    return Var(&_list->front(), TypeId::get<T>(), _is_const);
  };

  Expected<Var> back() override {
    if (_list->empty()) {
      return Error("The list is empty");
    }
    return Var(&_list->back(), TypeId::get<T>(), _is_const);
  };

 private:
  std::list<T>* _list;
  bool _is_const;
};

}  // namespace rr
