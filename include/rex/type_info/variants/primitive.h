#pragma once

#include <functional>
#include <stdexcept>

#include "../../variable/cell.h"
#include "fmt/format.h"
#include "rex/error/error.h"
#include "rex/expected.h"
#include "rex/info/type_id.h"
#include "rex/reflection/type_actions.h"
#include "rex/variant/variant.h"

namespace rr {

// clang-format off
#define BASE Variant<Cell<bool>, Cell<char>, Cell<float>, Cell<double>,\
                     Cell<int8_t>, Cell<uint8_t>, Cell<int16_t>, Cell<uint16_t>,\
                     Cell<int32_t>, Cell<uint32_t>, Cell<int64_t>, Cell<uint64_t>,\
                     Cell<std::string>, Cell<std::string_view>, Error>
// clang-format on

/**
 * The sum type with pointers to real values
 * supported types are:
 * signed and unsigned char as int8_t and uint8_t
 * plain char and C style strings via const char*
 * bool
 * float
 * double
 * int as int32_t
 * size_t as uint64_t
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
  Primitive(Cell<bool> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<char> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<float> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<double> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<int8_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<uint8_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<int16_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<uint16_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<int32_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<uint32_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<int64_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<uint64_t> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<std::string> value) : BASE(value) {  // NOLINT implicit constructor
  }

  Primitive(Cell<std::string_view> value) : BASE(value) {  // NOLINT implicit constructor
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