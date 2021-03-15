#pragma once

#include "tight_string.h"

namespace rr {

struct Error {
  explicit Error(const char* message) : _data(message) {
  }

  explicit Error(std::string_view message) : _data(message) {
  }

  explicit Error(std::string&& message) : _data(std::move(message)) {
  }

  Error(const Error& error) noexcept {
    _data = error._data;
  }

  Error& operator=(const Error& error) {
    _data = error._data;

    return *this;
  }

  Error(Error&& error) noexcept {

    _data = std::move(error._data);
  }

  Error& operator=(Error&& error) {
    _data = std::move(error._data);

    return *this;
  }

  // what() uses similar with plain exceptions interface
  // returns what happened
  std::string_view what() {

    if (_data.is_owned())
      return _data.owned;

    return _data.shared;
  }

 protected:
  TightString _data;
};

}  // namespace rr