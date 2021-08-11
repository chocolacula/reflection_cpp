#pragma once

#include <cstdarg>
#include <cstdio>

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

static void format(std::string* result, std::string_view fmt, size_t i) {
  *result += fmt.substr(i, fmt.size() - i);
}

template <typename T, typename... Ts>
static void format(std::string* result, std::string_view fmt, size_t i, T&& arg, Ts&&... args) {
  auto pos = fmt.find('{', i);

  if (pos == -1) {
    *result += fmt.substr(i, fmt.size() - i);
    return;
  } else {
    *result += fmt.substr(i, pos - i);
    *result += arg;
    format(result, fmt, pos + 2, args...);
  }
}

/// little format function with the single one purpose - format error messages with arguments
template <typename... Ts>
std::string format(std::string_view fmt, Ts&&... args) {

  std::string result;
  format(&result, fmt, 0, args...);
  return result;
}

}  // namespace rr
