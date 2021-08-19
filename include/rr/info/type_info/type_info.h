#pragma once

#include <string_view>

#include "../../expected.h"
#include "../../variant/variant.h"
#include "../field_info.h"
#include "variants/array/array.h"
#include "variants/bool.h"
#include "variants/enum/enum.h"
#include "variants/float.h"
#include "variants/integer.h"
#include "variants/map/map.h"
#include "variants/object.h"
#include "variants/sequence/sequence.h"
#include "variants/string.h"

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