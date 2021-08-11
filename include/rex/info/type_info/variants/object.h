#pragma once

#include <unordered_map>
#include <utility>

#include "rex/expected.h"
#include "rex/variable/var.h"

namespace rr {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Object {

  Object(Var var, std::unordered_map<std::string_view, Var>&& fields) : _var(var), _fields(fields) {
  }

  Expected<Var> get_field(std::string_view name) {

    auto it = _fields.find(name);

    if (it != _fields.end()) {
      return it->second;
    }

    return Error(format("There is no field with name: {}", name));
  }

  const std::unordered_map<std::string_view, Var>& get_all_fields() const {

    return _fields;
  }

  Var own_var() {
    return _var;
  }

 private:
  // TODO move it under shared_ptr
  std::unordered_map<std::string_view, Var> _fields;
  Var _var;
};

}  // namespace rr