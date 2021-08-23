#pragma once

#include <unordered_set>

#include "iset.h"

namespace rr {

template <typename T>
struct StdUnorderedSet : public ISet {
  StdUnorderedSet() = delete;

  explicit StdUnorderedSet(std::set<T>* set, bool is_const)
      : _set(set),  //
        _is_const(is_const) {
  }

  Var own_var() const override {
    return Var(_set, TypeId::get(_set), _is_const);
  }

  TypeId nested_type() const override {
    return TypeId::get<T>();
  }

  void for_each(std::function<void(Var)> callback) const override {
    for (auto&& entry : *_set) {
      // const values
      callback(Var(&entry));
    }
  }

  void clear() override {
    _set->clear();
  }

  size_t size() override {
    return _set->size();
  }

  Expected<None> push(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return Error(format("Trying to set with type: {} to unordered_set<{}>",  //
                          Reflection::type_name(value.type()),                 //
                          Reflection::type_name(nested_type)));
    }
    _set->insert(*static_cast<const T*>(value.raw()));
    return None();
  }

  Expected<None> remove(Var value) override {
    auto nested_type = TypeId::get<T>();

    if (nested_type != value.type()) {
      return Error(format("Cannot remove value with type: {} from unordered_set<{}>",  //
                          Reflection::type_name(value.type()),                         //
                          Reflection::type_name(nested_type)));
    }
    _set->pop();
    return None();
  }

  bool contains(Var value) override {
    auto it = _set->template find(*static_cast<const T*>(value.raw()));
    return it != _set->end();
  }

 private:
  std::unordered_set<T>* _set;
  bool _is_const;
};

}  // namespace rr
