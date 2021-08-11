#pragma once

#include <string_view>

#include "rex/expected.h"
#include "rex/info/field_info.h"
#include "rex/info/type_info/variants/array/array.h"
#include "rex/info/type_info/variants/bool.h"
#include "rex/info/type_info/variants/enum/enum.h"
#include "rex/info/type_info/variants/float.h"
#include "rex/info/type_info/variants/integer.h"
#include "rex/info/type_info/variants/map/map.h"
#include "rex/info/type_info/variants/object.h"
#include "rex/info/type_info/variants/sequence/sequence.h"
#include "rex/info/type_info/variants/string.h"
#include "rex/variant/variant.h"

namespace rr {

#define BASE Variant<Bool, Integer, Float, String, Enum, Object, Array, Sequence, Map>

/// The sum type contains information about nature of stored value
///
/// Primitive for int, float, std::string and others
/// Object for structs and classes
/// Sequence for arrays and containers with one generic parameter
/// Dictionary for associative container such as std::map
class TypeInfo : public BASE {
 public:
  TypeInfo(Bool value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Integer value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Float value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(String value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Enum value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Object value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Array value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Sequence value) : BASE(value) {  // NOLINT implicit constructor
  }

  TypeInfo(Map value) : BASE(value) {  // NOLINT implicit constructor
  }
};

#undef BASE

}  // namespace rr