/*
 * The sum type with possible values stored in FieldInfo
 * supported types are:
 * int as int32_t
 * float
 * double
 * int8_t
 * uint8_t
 * int16_t
 * uint16_t
 * int32_t
 * uint32_t
 * int64_t
 * uint64_t
 * std::string_view
 * Error
 */

#pragma once

#include <functional>
#include <stdexcept>

#include "error.h"
#include "overloaded.h"

namespace rr {

struct ExpectedValue {

  ExpectedValue(float value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(double value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(int8_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(uint8_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(int16_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(uint16_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(int32_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(uint32_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(int64_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(uint64_t value) : _content(value) {  // NOLINT implicit
  }

  ExpectedValue(std::string_view value) : _content(value) {  // NOLINT implicit
  }

  template <class... Ts>
  void unbind(Ts... functions) {
    std::visit(Overloaded{functions...}, _content);
  }

  template <class... Ts>
  void unbind_move(Ts... functions) {
    std::visit(Overloaded{functions...}, std::move(_content));
  }

  template <typename T>
  T get() {
    auto pointer = std::get_if<T>(&_content);

    if (pointer == nullptr) {
      throw std::runtime_error("Requested type doesn't exist in the ExpectedValue");
    }

    return *pointer;
  }

 private:
  std::variant<float, double,                                                             //
               int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t,  //
               std::string_view>
      _content;
};

}  // namespace rr