#pragma once

#include <functional>
#include <stdexcept>

#include "error.h"
#include "overloaded.h"

namespace rr {

template <typename T, typename ErrorT>
struct Expected {

  Expected(const T& value) : _content(value) {  // NOLINT implicit
  }

  Expected(T&& value) : _content(std::move(value)) {  // NOLINT implicit
  }

  Expected(const ErrorT& error) : _content(error) {  // NOLINT implicit
  }

  Expected(ErrorT&& error) : _content(std::move(error)) {  // NOLINT implicit
  }

  void unbind(std::function<void(T)> on_value, std::function<void(ErrorT)> on_error) {
    std::visit(Overloaded{on_value, on_error}, _content);
  }

  void unbind_move(std::function<void(T)> on_value, std::function<void(ErrorT)> on_error) {
    std::visit(Overloaded{on_value, on_error}, std::move(_content));
  }

  bool is_error() {
    return std::get_if<ErrorT>(&_content) != nullptr;
  }

  T get() {
    auto pointer = std::get_if<T>(&_content);

    if (pointer == nullptr) {
      throw std::runtime_error("It unexpectedly doesn't contain a value");
    }

    return *pointer;
  }

 private:
  std::variant<T, ErrorT> _content;
};

}  // namespace rr