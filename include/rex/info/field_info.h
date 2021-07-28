#pragma once

// #include <cstddef>
// #include <cstdint>
// #include <string_view>

// #include "../converter.h"
// #include "rex/expected.h"
// #include "type_id.h"
// #include "values/dictionary.h"
// #include "values/object.h"
// #include "values/primitive.h"
// #include "values/sequence.h"

namespace rr {

/// sum type which store information about accessibility and
/// few other types represent what the type is, such as
///
/// Primitive for int, float, string and others
/// Object for structs and classes
/// Sequence for array and containers with one generic parameter
/// Dictionary for associative container such as map
class FieldInfo {
 public:
  enum class Access {kPublic, kProtected, kPrivate};

  template <typename T>
  constexpr FieldInfo() {
  }

  // int id() const {
  //   return _type_id;
  // }

 private:
  Access _access;
};

}  // namespace rr