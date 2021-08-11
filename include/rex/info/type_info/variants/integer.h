#pragma once

#include "rex/expected.h"

namespace rr {

struct Integer {
  Integer(uint64_t* value, uint8_t size, bool is_signed, bool is_const)
      : _value(value), _size(size), _is_signed(is_signed), _is_const(is_const) {
  }

  bool is_signed() const {
    return _is_signed;
  }

  int64_t as_signed() const {
    switch (_size) {
      case 1:
        return *reinterpret_cast<int8_t*>(_value);
      case 2:
        return *reinterpret_cast<int16_t*>(_value);
      case 4:
        return *reinterpret_cast<int32_t*>(_value);
    }
    return *_value;
  }

  uint64_t as_unsigned() const {
    switch (_size) {
      case 1:
        return *reinterpret_cast<uint8_t*>(_value);
      case 2:
        return *reinterpret_cast<uint16_t*>(_value);
      case 4:
        return *reinterpret_cast<uint32_t*>(_value);
    }
    return *_value;
  }

  Expected<None> set(uint64_t value) {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    if (is_signed()) {
      switch (_size) {
        case 1:
          if (value > INT8_MAX) {
            return Error("The value too big to set signed 1 byte variable");
          }
          *reinterpret_cast<uint8_t*>(_value) = value;
          break;
        case 2:
          if (value > INT16_MAX) {
            return Error("The value too big to set signed 2 bytes variable");
          }
          *reinterpret_cast<uint16_t*>(_value) = value;
          break;
        case 4:
          if (value > INT32_MAX) {
            return Error("The value too big to set signed 4 bytes variable");
          }
          *reinterpret_cast<uint32_t*>(_value) = value;
          break;
        case 8:
          *_value = value;
          break;
      }
    } else {
      std::memcpy(_value, &value, _size);
    }
    return None();
  }

  Expected<None> set(int64_t value) {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    if (is_signed()) {
      switch (_size) {
        case 1:
          if (value > INT8_MAX || value < INT8_MIN) {
            return Error("The value too big to set 1 byte variable");
          }
          *reinterpret_cast<int8_t*>(_value) = value;
          break;
        case 2:
          if (value > INT16_MAX || value < INT16_MIN) {
            return Error("The value too big to set 2 bytes variable");
          }
          *reinterpret_cast<int16_t*>(_value) = value;
          break;
        case 4:
          if (value > INT32_MAX || value < INT32_MIN) {
            return Error("The value too big to set 4 bytes variable");
          }
          *reinterpret_cast<int32_t*>(_value) = value;
          break;
        case 8:
          *_value = value;
          break;
      }
    } else {
      if (value < 0) {
        return Error("Cannot assign signed value to unsigned");
      }
      std::memcpy(_value, &value, _size);
    }
    return None();
  }

  std::string to_string() const {
    if (is_signed()) {
      return std::to_string(as_signed());
    }
    return std::to_string(*_value);
  }

 private:
  uint64_t* _value;
  const uint8_t _size;
  const bool _is_signed;
  const bool _is_const;
};

}  // namespace rr