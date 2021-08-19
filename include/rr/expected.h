#pragma once

#include <stdexcept>
#include <variant>

#include "error/error.h"
#include "variant/variant.h"

namespace rr {

#define BASE Variant<T, Error>

template <typename T>
struct Expected : public BASE {

  Expected(const T& value) : BASE(value) {  // NOLINT implicit constructor
  }

  Expected(T&& value) : BASE(std::move(value)) {  // NOLINT implicit constructor
  }

  Expected(const Error& error) : BASE(error) {  // NOLINT implicit constructor
  }

  Expected(Error&& error) : BASE(std::move(error)) {  // NOLINT implicit constructor
  }

  Expected(const Expected& other) noexcept : BASE::_content(other._content) {
  }

  Expected& operator=(const Expected& other) {
    if (this == &other) {
      return *this;
    }

    BASE::_content = other._content;

    return *this;
  }

  Expected(Expected&& other) noexcept : BASE::_content(std::move(other._content)) {
  }

  Expected& operator=(Expected&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    BASE::_content = std::move(other._content);

    return *this;
  }

  bool is_error() {
    return BASE::template is<Error>();
  }

  T unwrap() {
    auto pointer = std::get_if<T>(&(BASE::_content));

    if (pointer == nullptr) {
      throw std::runtime_error(std::get<Error>(BASE::_content).what().data());
    }
    return *pointer;
  }
};

#undef BASE

}  // namespace rr