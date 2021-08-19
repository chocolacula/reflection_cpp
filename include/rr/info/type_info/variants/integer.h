#pragma once

#include "../../../expected.h"
#include "../../../tools/format.h"

namespace rr {

struct Integer {
  Integer(uint64_t* value, uint8_t size, bool is_signed, bool is_const)
      : _value(value), _size(size), _is_signed(is_signed), _is_const(is_const) {
  }

  bool is_signed() const {
    return _is_signed;
  }

  int64_t get() const {
    switch (_size) {
      case sizeof(int8_t):
        return *reinterpret_cast<int8_t*>(_value);

      case sizeof(int16_t):
        return *reinterpret_cast<int16_t*>(_value);

      case sizeof(int32_t):
        return *reinterpret_cast<int32_t*>(_value);
    }
    return *reinterpret_cast<int64_t*>(_value);
  }

  Expected<None> set(int64_t value) {
    if (_is_const) {
      return Error("Trying to set const value");
    }
    if (!is_fit(value)) {
      return Error(format("The value is too big to fit {} byte variable", _size));
    }
    if (is_signed()) {
      switch (_size) {
        case sizeof(int8_t):
          *reinterpret_cast<int8_t*>(_value) = value;
          break;

        case sizeof(int16_t):
          *reinterpret_cast<int16_t*>(_value) = value;
          break;

        case sizeof(int32_t):
          *reinterpret_cast<int32_t*>(_value) = value;
          break;
      }
      *_value = value;
    } else {
      if (value < 0) {
        return Error("Cannot assign negative value to unsigned");
      }
      switch (_size) {
        case sizeof(uint8_t):
          *reinterpret_cast<uint8_t*>(_value) = value;
          break;

        case sizeof(uint16_t):
          *reinterpret_cast<uint16_t*>(_value) = value;
          break;

        case sizeof(uint32_t):
          *reinterpret_cast<uint32_t*>(_value) = value;
          break;
      };
      *_value = value;
    }
    return None();
  }

  std::string to_string() const {
    if (is_signed()) {
      return std::to_string(get());
    }
    return std::to_string(*_value);
  }

 private:
  uint64_t* _value;
  const uint8_t _size;
  const bool _is_signed;
  const bool _is_const;

  inline bool is_neg() const {
    int64_t v = *reinterpret_cast<int64_t*>(_value);
    return (v << (64U - _size * 8)) < 0;
  }

  inline bool is_fit(uint64_t v) const {
    if (_size == sizeof(uint64_t)) {
      return true;
    }
    uint64_t mask = -1;
    mask = mask << (_size * 8U);
    return (mask & v) == 0;
  }
};

}  // namespace rr