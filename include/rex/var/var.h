#pragma once

#include <fmt/format.h>

#include "../expected.h"
#include "../info/type_id.h"

namespace rr {

struct Var {

  template <typename T>
  explicit Var(const T* value) : _value(const_cast<T*>(value)), _type(TypeId::get<T>()), _is_const(true) {
  }

  template <typename T>
  explicit Var(T* value) : _value(value), _type(TypeId::get(value)), _is_const(false) {
  }

  Var(void* value, TypeId type) : _value(value), _type(type) {
  }

  bool operator==(const Var& other) const {
    return _type == other._type && _value == other._value;
  }

  bool operator!=(const Var& other) const {
    return _type != other._type || _value != other._value;
  }

  void* raw() const {
    return _value;
  }

  bool is_const() const {
    return _is_const;
  }

  /// runtime type check and cast
  template <typename T>
  Expected<T*> rt_cast() const {

    auto desired_type = TypeId::get<T>();

    if (desired_type != _type) {
      return Error(fmt::format("Cannot cast {} to {}",         //
                               TypeActions::type_name(_type),  //
                               TypeActions::type_name(desired_type)));
    }

    return static_cast<T*>(_value);
  }

  TypeId type() const {
    return _type;
  }

 protected:
  void* _value;
  TypeId _type;
  bool _is_const;
};

}