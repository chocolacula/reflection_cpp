#pragma once

#include <string_view>

#include "rex/expected.h"
#include "rex/info/field_info.h"
#include "rex/type_info/variants/object.h"
#include "rex/type_info/variants/primitive.h"
#include "rex/type_info/variants/sequence/sequence.h"
#include "rex/variant/variant.h"

namespace rr {

#define BASE Variant<Primitive, Object, Sequence>

/// The sum type contains information about nature of stored value
///
/// Primitive for int, float, std::string and others
/// Object for structs and classes
/// Sequence for arrays and containers with one generic parameter
/// Dictionary for associative container such as std::map
class TypeInfo : public BASE {
 public:

  TypeInfo(Primitive value) : BASE(value) { // NOLINT implicit constructor
  }

  TypeInfo(Object value) : BASE(value) { // NOLINT implicit constructor
  }

  TypeInfo(Sequence value) : BASE(value) { // NOLINT implicit constructor
  }

  // Expected<None> serialize(const void* object, Serializer* adapter) const {
  //   return Error("Unknown type");
  // }
  //
  // Expected<None> deserialize(void* object, Serializer* adapter) const {
  //   return Error("Unknown type");
  // }
};

#undef BASE

}  // namespace rr