#pragma once

#include <utility>
#include <unordered_map>

#include "rex/var/var.h"
#include "rex/expected.h"
#include "fmt/format.h"

namespace rr {

/// container of all field names matched to Var structs
/// just a registry of types and value pointers
struct Object {

  Object(Var var, std::unordered_map<std::string, Var>&& fields) : _var(var), _fields(fields) {
  }

  Expected<Var> get_field(std::string_view name) {

    auto it = _fields.find(std::string(name));

    if (it != _fields.end()) {
      return it->second;
    }

    return Error(fmt::format("There is no field with name: {}", name));
  }

  const std::unordered_map<std::string, Var>& get_all_fields() {

    return _fields;
  }

  Var var() {
    return _var;
  }

 private:
  std::unordered_map<std::string, Var> _fields;
  Var _var;
};

} // namespace rr