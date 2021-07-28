#pragma once

#include "rex/info/type_id.h"

namespace rr {

struct Var {

  template <typename T>
  explicit Var(T* value) : _value(value), _type(TypeId::get(value)) {
  }

  template <typename T>
  explicit Var(T&& value) : _value(&value), _type(TypeId::get(&value)) {
  }

  Var(void* value, TypeId type) : _value(value), _type(type) {
  }

  bool operator==(const Var& other) const {
    return _type == other._type && _value == other._value;
  }

  bool operator!=(const Var& other) const {
    return _type != other._type || _value != other._value;
  }

  void* value() const {
    return _value;
  }

  TypeId type() const {
    return _type;
  }

 protected:
    void* _value;
    TypeId _type;
};

}