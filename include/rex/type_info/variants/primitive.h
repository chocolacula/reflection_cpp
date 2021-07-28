#pragma once

#include <functional>
#include <stdexcept>

#include "fmt/format.h"
#include "rex/error/error.h"
#include "rex/expected.h"
#include "rex/info/type_id.h"
#include "rex/reflection/type_actions.h"
#include "rex/variant/variant.h"

namespace rr {

#define BASE                                                                                                 \
  Variant<float*, double*, int8_t*, uint8_t*, int16_t*, uint16_t*, int32_t*, uint32_t*, int64_t*, uint64_t*, \
          std::string*, std::string_view*, Error>

/**
 * The sum type with pointers to real values
 * supported types are:
 * signed and unsigned char as int8_t and uint8_t
 * int as int32_t
 * size_t as uint64_t
 * float
 * double
 * int8_t
 * uint8_t
 * int16_t
 * uint16_t
 * int32_t
 * uint32_t
 * int64_t
 * uint64_t
 * std::string and std::string_view
 * Error as handler for others unsupported types
 */
struct Primitive : public BASE {
  Primitive(float* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(double* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(int8_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(uint8_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(int16_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(uint16_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(int32_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(uint32_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(int64_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(uint64_t* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(std::string* value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(std::string_view* value) : BASE(value) {  // NOLINT implicit constructor
  }

  template <typename T>
  Primitive(T* value) : BASE(Error("Unknown primitive type")) {  // NOLINT implicit constructor
  }

  template <typename T>
  Expected<None> set(const T& value) {

    std::string err_str;

    BASE::match([&](T* v) { *v = value; },
                [&err_str](auto&& v) {
                  err_str = fmt::format("Trying to set value({}) to primitive({})",  //
                                        TypeActions::type_name(TypeId::get<T>()),    //
                                        TypeActions::type_name(TypeId::get(v)));
                });

    if (!err_str.empty()) {
      return Error(std::move(err_str));
    }

    return None();
  }
};

#undef BASE

}  // namespace rr