#pragma once

#include <functional>
#include <string_view>

#include "../expected/expected_value.h"
#include "type_id.h"

namespace rr {

class Converter {
 public:
  static ExpectedValue from_pointer(const char* pointer, int type_id) {
    return (*kFunctions[type_id])(pointer);
  }

 private:
  static ExpectedValue cast_float(const char* pointer) {
    auto value = *reinterpret_cast<const float*>(pointer);
    return value;
  }

  static ExpectedValue cast_double(const char* pointer) {
    auto value = *reinterpret_cast<const double*>(pointer);
    return value;
  }

  static ExpectedValue cast_int8_t(const char* pointer) {
    auto value = *reinterpret_cast<const int8_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint8_t(const char* pointer) {
    auto value = *reinterpret_cast<const uint8_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_int16_t(const char* pointer) {
    auto value = *reinterpret_cast<const int16_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint16_t(const char* pointer) {
    auto value = *reinterpret_cast<const uint16_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_int32_t(const char* pointer) {
    auto value = *reinterpret_cast<const int32_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint32_t(const char* pointer) {
    auto value = *reinterpret_cast<const uint32_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_int64_t(const char* pointer) {
    auto value = *reinterpret_cast<const int64_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint64_t(const char* pointer) {
    auto value = *reinterpret_cast<const uint64_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_string(const char* pointer) {
    const auto* str_p = reinterpret_cast<const std::string*>(pointer);

    return std::string_view(str_p->c_str());
  }

  static ExpectedValue cast_cstring(const char* pointer) {

    return std::string_view(pointer);
  }

  static ExpectedValue cast_string_view(const char* pointer) {
    auto value = *reinterpret_cast<const std::string_view*>(pointer);
    return value;
  }

 public:
  static constexpr inline ExpectedValue (*kFunctions[13])(const char*) = {
      cast_float,        //
      cast_double,       //
      cast_int8_t,       //
      cast_uint8_t,      //
      cast_int16_t,      //
      cast_uint16_t,     //
      cast_int32_t,      //
      cast_uint32_t,     //
      cast_int64_t,      //
      cast_uint64_t,     //
      cast_string,       //
      cast_cstring,      //
      cast_string_view,  //
  };
};

}  // namespace rr