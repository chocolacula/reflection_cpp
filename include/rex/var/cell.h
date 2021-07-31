#pragma once

#include "../info/type_id.h"

namespace rr {

template <typename T>
struct Cell {

  Cell(T* value) : _value(value), _is_const(false) {
  }

  Cell(T* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  Cell(const T* value) : _value(const_cast<T*>(value)), _is_const(true) {
  }

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

 protected:
  T* _value;
  bool _is_const;
};

}  // namespace rr