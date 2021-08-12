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
    std::string result;
    if (_size == 4) {
      result = std::to_string(*reinterpret_cast<float*>(_value));
    } else {
      result = std::to_string(*_value);
    }
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);

    if (result[result.length() - 1] == '.') {
      result.pop_back();
    }
    return result;
  }

 private:
  double* _value;
  const uint8_t _size;
  const bool _is_const;
};

}  // namespace rr