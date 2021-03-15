#pragma once

#include <string_view>

#include "field_info.h"
#include "rex/expected/error.h"
#include "rex/expected/expected.h"

namespace rr {

template <typename T>
class TypeInfo {
 public:
  TypeInfo() = default;

  static Expected<FieldInfo, Error> get_field(std::string_view name) {

    return Error("Unknown enum type");
  }
};

class Serializer;

struct TypeInfoProxy {
  Expected<FieldInfo, Error> (*get_field)(std::string_view);
  Expected<std::monostate, Error> (*serialize)(const void* object, Serializer* adapter);
  Expected<std::monostate, Error> (*deserialize)(void* object, Serializer* adapter);
};

template <>
class TypeInfo<void> {
 public:
  TypeInfo() = default;

  TypeInfo(int type_id,  //
           Expected<FieldInfo, Error> (*get_field)(std::string_view),
           Expected<std::monostate, Error> (*serialize)(const void* object, Serializer* adapter),
           Expected<std::monostate, Error> (*deserialize)(void* object, Serializer* adapter))
      : _proxy{get_field, serialize, deserialize} {
    kProxies[type_id] = *this;
  }

  Expected<FieldInfo, Error> get_field(std::string_view name) const {
    return _proxy.get_field(name);
  }

  Expected<std::monostate, Error> serialize(const void* object, Serializer* adapter) const {
    return _proxy.serialize(object, adapter);
  }

  Expected<std::monostate, Error> deserialize(void* object, Serializer* adapter) const {
    return _proxy.deserialize(object, adapter);
  }

 private:
  friend TypeInfo<void>& type_info(int type_id);
  TypeInfoProxy _proxy;

  // static storage of all registered TypeInfo<void>
  static TypeInfo<void> kProxies[TypeId::max_id()];
};

TypeInfo<void> TypeInfo<void>::kProxies[TypeId::max_id()] = {};

TypeInfo<void>& type_info(int type_id) {
  return TypeInfo<void>::kProxies[type_id];
}

}  // namespace rr