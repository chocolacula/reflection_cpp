#pragma once

namespace rr {

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