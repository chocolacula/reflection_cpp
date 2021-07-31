#pragma once

#include "../info/type_id.h"

namespace rr {

class TypeActions {
  static inline std::string_view type_name(TypeId id);
  static inline size_t type_size(TypeId id);

  friend class Reflection;
  friend class Primitive;
  friend class Sequence;
  friend class Var;
  friend class ConstVar;
};

}  // namespace rr