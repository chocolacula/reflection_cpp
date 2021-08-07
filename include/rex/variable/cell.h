#pragma once

#include "../info/type_id.h"

namespace rr {

template <typename T>
struct Cell {

  explicit Cell(T* value) : _value(value), _is_const(false) {
  }

  explicit Cell(const T* value) : _value(const_cast<T*>(value)), _is_const(true) {
  }

  Cell(T* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  // explicit Cell(const Var& var) : _value(const_cast<T*>(var.raw())), _is_const(var.is_const()) {
  // }

  bool is_const() const {
    return _is_const;
  }

  T* ptr_mut() {
    if (_is_const) {
      return nullptr;
    }

    return _value;
  }

  const T* ptr() {
    return _value;
  }

  // Var var() {
  //   return Var(_value, TypeId::get<T>(), _is_const);
  // }

 private:
  T* _value;
  bool _is_const;

  friend class Var;
};

}  // namespace rr