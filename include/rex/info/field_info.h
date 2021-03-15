#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

#include "../expected/expected.h"
#include "../expected/expected_value.h"
#include "converter.h"
#include "type_id.h"

namespace rr {

class FieldInfo {
 public:
  constexpr FieldInfo(size_t offset, size_t type) : _offset(offset), _type_id(type) {
  }

  FieldInfo(const FieldInfo& info) = default;

  FieldInfo& operator=(const FieldInfo& info) {
    _offset = info._offset;
    _type_id = info._type_id;

    return *this;
  }

  ExpectedValue get(const void* object) const {

    const char* p = reinterpret_cast<const char*>(object) + _offset;

    return Converter::from_pointer(p, _type_id);
  }

  template <typename T>
  Expected<std::monostate, Error> set(void* object, T value) {

    auto type_id = TypeId::get<T>();

    if (type_id != _type_id) {
      return Error("A try to set different type value");
    }

    char* p = reinterpret_cast<char*>(object) + _offset;
    *reinterpret_cast<T*>(p) = value;

    return std::monostate();
  }

  [[nodiscard]] int id() const {
    return _type_id;
  }

 private:
  size_t _offset;
  int _type_id;
};

}  // namespace rr