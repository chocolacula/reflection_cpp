#pragma once

#include "access.h"

namespace rr {

class FieldInfo {
 public:
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