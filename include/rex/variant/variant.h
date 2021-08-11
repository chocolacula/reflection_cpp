#pragma once

#include <variant>

#include "overloaded.h"
#include "rex/expected.h"

namespace rr {

// declaring a slightly more readable state of nothing
using None = std::monostate;

template <typename... T>
struct Variant {

  template <typename SomeT>
  Variant(const SomeT& value) : _content(value) {  // NOLINT implicit constructor
  }

  template <typename SomeT>
  Variant(SomeT&& value) : _content(std::move(value)) {  // NOLINT implicit constructor
  }

  template <typename... FuncT>
  decltype(auto) match(FuncT... functions) {
    return std::visit(Overloaded{functions...}, _content);
  }

  template <typename... FuncT>
  decltype(auto) match(FuncT... functions) const {
    return std::visit(Overloaded{functions...}, _content);
  }

  template <typename SomeT>
  bool is() {
    return std::holds_alternative<SomeT>(_content);
  }

  template <typename SomeT>
  SomeT& get() {
    auto pointer = std::get_if<SomeT>(&_content);

    if (pointer == nullptr) {
      throw std::runtime_error("The instance of requested type doesn't exist");
    }
    return *pointer;
  }

 protected:
  std::variant<T...> _content;
};

}  // namespace rr