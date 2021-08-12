#pragma once

#include "traits.h"

namespace rr {

#ifdef __GNUG__

#include <cxxabi.h>

template <typename T>
std::string_view constexpr rr_type_name() {
  int status = -4;
  return abi::__cxa_demangle(typeid(T).name(), NULL, NULL, &status);
}

#else

template <typename T>
std::string_view constexpr rr_type_name() {
  return typeid(T).name();
}

#endif

constexpr size_t align_sizeof(size_t alignment, size_t raw_size) {
  size_t mul = raw_size / alignment;
  auto rest = raw_size % alignment;
  if (rest != 0) {
    mul += 1;
  }
  return mul * alignment;
}

}  // namespace rr
