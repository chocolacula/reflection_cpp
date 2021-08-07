#pragma once

#include <string_view>

#include "rex/error/error.h"
#include "rex/expected.h"

namespace rr {

template <typename T>
class EnumInfo {
 public:
  static Expected<std::string_view> to_string(const T /*constant*/) {
    return Error("Unknown enum type");
  }

  static Expected<T> parse(std::string_view /*name*/) {
    return Error("Unknown enum type");
  }
};

}  // namespace rr
