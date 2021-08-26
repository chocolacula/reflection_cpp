#pragma once

#include <string_view>

#include "error.h"
#include "position.h"

namespace rr {

struct ErrorParse : public Error {
 public:
  ErrorParse(const char* message, Position position) : Error(message), _position(position) {
  }

  ErrorParse(std::string_view message, Position position) : Error(message), _position(position) {
  }

  ErrorParse(std::string&& message, Position position) : Error(std::move(message)), _position(position) {
  }

  ErrorParse(const ErrorParse& other) = default;

  ErrorParse& operator=(const ErrorParse& other) {
    if (this == &other) {
      return *this;
    }

    _data = other._data;
    _position = other._position;

    return *this;
  }

  ErrorParse(ErrorParse&& other) = default;

  ErrorParse& operator=(ErrorParse&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    _data = std::move(other._data);
    _position = other._position;

    return *this;
  }

  Position get_position() {
    return _position;
  }

 private:
  Position _position;
};

}  // namespace rr