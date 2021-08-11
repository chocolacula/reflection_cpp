#pragma once

#include <cfloat>

namespace rr {

struct Float {
  Float(double* value, uint8_t size, bool is_const) : _value(value), _size(size), _is_const(is_const) {
  }

  double get() {
    if (_size == 4) {
      return *reinterpret_cast<float*>(_value);
    }
    return *_value;
  }

  Expected<None> set(double value) {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    if (_size == 4) {
      if (value > FLT_MAX || value < FLT_MIN) {
        return Error("The value too big to set float variable");
      }
      *reinterpret_cast<float*>(_value) = value;
    } else {
      *_value = value;
    }
    return None();
  }

  std::string to_string() const {
    if (_size == 4) {
      return std::to_string(*reinterpret_cast<float*>(_value));
    }
    return std::to_string(*_value);
  }

 private:
  double* _value;
  const uint8_t _size;
  const bool _is_const;
};

}  // namespace rr