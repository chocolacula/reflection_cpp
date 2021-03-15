#pragma once

#include <string_view>

#include "rex/expected/error.h"
#include "rex/expected/expected.h"

namespace rr {

template <typename T>
class EnumInfo {
 public:
  static Expected<std::string_view, Error> to_string(const T constant) {
    return Error("Unknown enum type");
  }

  static Expected<T, Error> parse(std::string_view name) {
    return Error("Unknown enum type");
  }
};

}  // namespace rr
