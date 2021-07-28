#pragma once

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <cstddef>
#include <string_view>
#include <type_traits>

#include "rex/expected.h"
#include "rex/expected/error.h"
#include "rex/info/field_info.h"
#include "rex/serializer/serializer.h"
#include "rex/type_info/type_info.h"

// from input parameter
#include "../../object.h"

namespace rr {

template <>
constexpr int TypeId::get<TheStruct2>() {
  constexpr int id = kId + 1;
  return id;
}

template <>
class TypeInfo<TheStruct2> {
  static_assert(
      std::is_standard_layout<TheStruct2>::value,
      "To ensure fields of the object could be accessed right it must be an object with standard memory layout");

 public:
  static Expected<FieldInfo, Error> get_field(std::string_view name) {

    if (kFields.count(name) != 0) {
      return kFields.at(name);
    }

    return Error("Cannot find a field with requested name");
  }

  static Expected<std::monostate, Error> serialize(const TheStruct2* object, Serializer* adapter) {

    for (auto&& [name, info] : kFields) {
      adapter->from_field(name.data(), info, object);
    }

    return std::monostate();
  }

  static Expected<std::monostate, Error> deserialize(TheStruct2* object, Serializer* adapter) {

    for (auto&& [name, info] : kFields) {
      adapter->to_field(name.data(), info, object);
    }

    return std::monostate();
  }

 private:
  static constexpr frozen::unordered_map<frozen::string, FieldInfo, 2> kFields = {
      {"host", FieldInfo{offsetof(TheStruct2, _host), TypeId::get<std::string>()}},
      {"server_port", FieldInfo{offsetof(TheStruct2, _port), TypeId::get<int>()}},
  };
};

}  // namespace rr