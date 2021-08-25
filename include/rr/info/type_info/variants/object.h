#pragma once

#include <iostream>
#include <map>
#include <utility>

#include "../../../expected.h"
#include "../../../variable/var.h"

namespace rr {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Object {

  Object(Var var, std::map<std::string_view, Var>&& fields) : _fields(fields), _var(var) {
  }

  Expected<Var> get_field(std::string_view name) {
    auto it = _fields.find(name);

    if (it != _fields.end()) {
      return it->second;
    }

    return Error(format("There is no field with name: {}", name));
  }

  const std::map<std::string_view, Var>& get_all_fields() const {

    return _fields;
  }

  Var own_var() {
    return _var;
  }

 private:
  std::map<std::string_view, Var> _fields;
  Var _var;
};

}  // namespace rr