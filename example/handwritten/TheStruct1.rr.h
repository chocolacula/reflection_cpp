#pragma once

#include <frozen/string.h>
#include <frozen/unordered_map.h>

#include <cstddef>
#include <string_view>
#include <type_traits>

#include "rex/expected/error.h"
#include "rex/expected/expected.h"
#include "rex/info/field_info.h"
#include "rex/info/type_info.h"
#include "rex/serializer/serializer.h"

// from input parameter
#include "../object.h"

namespace rr {

template <>
constexpr int TypeId::get<TheStruct1>() {
  constexpr int id = kId + 1;
  return id;
}

template <>
class TypeInfo<TheStruct1> {
  static_assert(
      std::is_standard_layout<TheStruct1>::value,
      "To ensure fields of the object could be accessed right it must be an object with standard memory layout");

 public:
  static Expected<FieldInfo, Error> get_field(std::string_view name) {

    if (kFields.count(name) != 0) {
      return kFields.at(name);
    }

    return Error("Cannot find a field with requested name");
  }

  static Expected<std::monostate, Error> serialize(const void* object, Serializer* adapter) {

    for (auto&& [name, info] : kFields) {
      adapter->from_field(name.data(), info, object);
    }

    return std::monostate();
  }

  static Expected<std::monostate, Error> deserialize(void* object, Serializer* adapter) {

    for (auto&& [name, info] : kFields) {
      adapter->to_field(name.data(), info, object);

      // or
      // TypeInfo<BlahBlah>::deserialize(nested, new_serializer)
    }

    return std::monostate();
  }

 private:
  static inline TypeInfo<void> registrator = {TypeId::get<TheStruct1>(),         //
                                              &TypeInfo<TheStruct1>::get_field,  //
                                              &TypeInfo<TheStruct1>::serialize,  //
                                              &TypeInfo<TheStruct1>::deserialize};

  static constexpr frozen::unordered_map<frozen::string, FieldInfo, 2> kFields = {
      {"host", FieldInfo{offsetof(TheStruct1, host), TypeId::get<std::string>()}},
      {"server_port", FieldInfo{offsetof(TheStruct1, port), TypeId::get<int>()}},
  };
};

}  // namespace rr