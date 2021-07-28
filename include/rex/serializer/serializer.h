#pragma once

#include <string_view>

#include "../info/field_info.h"

namespace rr {

class Serializer {
 public:
  virtual void to_field(const char* name, FieldInfo info) = 0;

  virtual void from_field(const char* name, FieldInfo info) = 0;
};

}  // namespace rr