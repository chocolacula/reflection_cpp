#pragma once

#include <variant>

#include "overloaded.h"
#include "rex/expected.h"

namespace rr {

// declaring a slightly more readable state of nothing
using None = std::monostate;

template <typename ...T>
struct Variant {

  template <typename SomeT>
  Variant(const SomeT& value) : _content(value) {  // NOLINT implicit constructor
  }

  template <typename SomeT>
  Variant(SomeT&& value) : _content(std::move(value)) {  // NOLINT implicit constructor
  }

  template <typename... FuncT>
  void match(FuncT... functions) {
    std::visit(Overloaded{ functions... }, _content);
  }

  template <typename... FuncT>
  void match_move(FuncT... functions) {
    std::visit(Overloaded{ functions... }, std::move(_content));
  }

  // void unbind(std::function<void(T)> on_value, std::function<void(ErrorT)> on_error) {
  //   std::visit(Overloaded{on_value, on_error}, _content);
  // }
  //
  // void unbind_move(std::function<void(T)> on_value, std::function<void(ErrorT)> on_error) {
  //   std::visit(Overloaded{on_value, on_error}, std::move(_content));
  // }

  template <typename SomeT>
  SomeT get() {
    auto pointer = std::get_if<SomeT>(&_content);

    if (pointer == nullptr) {
      throw std::runtime_error("Requested type doesn't exist in the Primitive");
    }

    return *pointer;
  }

 protected:
  std::variant<T...> _content;
};

} // namespace rr