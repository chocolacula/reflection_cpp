#pragma once

#include <fmt/format.h>

#include "../expected.h"
#include "../info/type_id.h"

namespace rr {

/// The abstraction from type and const modifier
/// all types represented like a pointer + type id + const flag
/// the same representation for each type
struct Var {

  template <typename T>
  explicit Var(const T* value) : _value(const_cast<T*>(value)), _type(TypeId::get<T>()), _is_const(true) {
  }

  template <typename T>
  explicit Var(T* value) : _value(value), _type(TypeId::get(value)), _is_const(false) {
  }

  Var(void* value, TypeId type, bool is_const) : _value(value), _type(type), _is_const(is_const) {
  }

  bool operator==(const Var& other) const {
    return _type == other._type && _value == other._value;
  }

  bool operator!=(const Var& other) const {
    return _type != other._type || _value != other._value;
  }

  bool is_const() const {
    return _is_const;
  }

  void* raw_mut() const {
    if (_is_const) {
      return nullptr;
    }

    return _value;
  }

  const void* raw() const {
    return _value;
  }

  /// runtime type check and cast
  template <typename T>
  Expected<T*> rt_cast() const {

    if (std::is_const_v<T> == false && _is_const) {
      return Error("The type under Var has const qualifier, cannot cast to mutable");
    }

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

 private:
  void* _value;
  TypeId _type;
  bool _is_const;
};

}  // namespace rr