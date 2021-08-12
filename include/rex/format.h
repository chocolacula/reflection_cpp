#pragma once

#include "traits.h"

namespace rr {

template <typename T>
static typename std::enable_if_t<is_string_v<std::remove_reference_t<T>>, void>  //
append(std::string* str, T&& arg) {
  *str += arg;
}

template <typename T>
static typename std::enable_if_t<std::is_array_v<std::remove_reference_t<T>> &&  //
                                     std::is_same_v<array_value_t<T>, const char>,
                                 void>
append(std::string* str, T&& arg) {
  *str += arg;
}

template <typename T>
static typename std::enable_if_t<is_number_v<std::remove_reference_t<T>>, void>  //
append(std::string* str, T&& arg) {
  *str += std::to_string(arg);
}

template <typename T>
static typename std::enable_if_t<std::is_same_v<std::remove_reference_t<T>, bool>, void>  //
append(std::string* str, T&& arg) {
  *str += arg ? "true" : "false";
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
  }
  *result += fmt.substr(i, pos - i);
  append(result, std::forward<T>(arg));
  format(result, fmt, pos + 2, std::forward<Ts>(args)...);
}

/// little format function with the single one purpose - format error messages with arguments
template <typename... Ts>
std::string format(std::string_view fmt, Ts&&... args) {
  std::string result;
  format(&result, fmt, 0, std::forward<Ts>(args)...);
  return result;
}

}  // namespace rr
