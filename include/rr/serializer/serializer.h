#pragma once

#include <string_view>

#include "../info/field_info.h"

namespace rr {

class Native {
 public:
  virtual void serialize(const FieldInfo& info) = 0;
  virtual void deserialize(FieldInfo* info) = 0;
};

}  // namespace rr