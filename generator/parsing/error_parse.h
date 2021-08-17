#pragma once

#include <string_view>

#include "position.h"
#include "rex/error/error.h"

struct ErrorParse : public rr::Error {
 public:
  ErrorParse(const char* message, Position position) : Error(message), _position(position) {
  }

  ErrorParse(std::string_view message, Position position) : Error(message), _position(position) {
  }

  ErrorParse(std::string&& message, Position position) : Error(std::move(message)), _position(position) {
  }

  Position get_position() {
    return _position;
  }

 private:
  Position _position;
};