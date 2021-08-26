#pragma once

#include <stdexcept>
#include <variant>

#include "error/error.h"
#include "variant/variant.h"

namespace rr {

#define BASE Variant<T, ErrorT>

template <typename T, typename ErrorT = Error>
struct Expected : public BASE {

  template <typename = std::enable_if_t<!std::is_same_v<T, Expected>, void>>
  Expected(const T& value) : BASE(value) {  // NOLINT implicit constructor
  }

  template <typename = std::enable_if_t<!std::is_same_v<T, Expected>, void>>
  Expected(T&& value) : BASE(std::move(value)) {  // NOLINT implicit constructor
  }

  template <typename = std::enable_if_t<!std::is_same_v<ErrorT, Expected>, void>>
  Expected(const ErrorT& error) : BASE(error) {  // NOLINT implicit constructor
  }

  template <typename = std::enable_if_t<!std::is_same_v<ErrorT, Expected>, void>>
  Expected(ErrorT&& error) : BASE(std::move(error)) {  // NOLINT implicit constructor
  }

  bool is_error() {
    return BASE::template is<ErrorT>();
  }

  T unwrap() {
    auto pointer = std::get_if<T>(&(BASE::_content));

    if (pointer == nullptr) {
      throw std::runtime_error(std::get<ErrorT>(BASE::_content).what().data());
    }
    return *pointer;
  }
};

#undef BASE

}  // namespace rr