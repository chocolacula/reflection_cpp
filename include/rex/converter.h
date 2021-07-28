#pragma once

#include <functional>
#include <string_view>

#include "../expected/expected_value.h"
#include "cursor/reader.h"
#include "info/type_id.h"

namespace rr {

class Converter {
 public:
  static ExpectedValue to_primitive(void* pointer, int type_id) {
    return (*kFunctions[type_id])(pointer);
  }

  static Expected<None, Error> to_object(TypeInfo object, IReader<std::string_view> reader) {

    TypeInfo.match(
      [](Primitive p) {
          p.match(
            [](int* v){ *v = reader.move(p.name()).read<int>(); },
            [](float* v){ *v = reader.move(p.name()).read<float>(); },
            [](double * v){ *v = reader.move(p.name()).read<double>(); }
          );
      },
      [](Object o) {

          for (auto&& id : o.fields()) {
            auto info = Reflection::make_reflection(o.type());
            to_object(info, reader.move(v.name()));
          }
      },
      // it has capacity, and size
      [](Sequence s) {

          auto arr = reader.move(s.name());

          if (arr.is_single())
            return Error();

          for (auto it = arr.begin(); it != arr.end(); it++) {
            // disassemble nested object no matter what is

            // create default value
            auto value = Box(s.type());

            //or Reflection::Box(s.type(), ...);

            // serialize it via reflection
            auto valueInfo = Reflection::reflect(value.var);
            to_object(&valueInfo, *it);

            // move to a sequence
            s.add(value.var);
          }
        },
      [](Dictionary d) {

        auto arr = reader.move(d.name());

        if (arr.is_single())
          return Error();

        for (auto it = arr.begin(); it != arr.end(); it++) {
          // disassemble nested object no matter what is

          //create default key
          auto key = Reflection::create_default(d.first_type());
          auto keyInfo = Reflection::reflect(key, d.first_type());

          to_object(&keyInfo, reader.move("key"));

          Var value = d.alloc_value(std::move(key));
          auto valueInfo = Reflection::reflect(value.value, value.type());

          to_object(&valueInfo, reader.move("value"));

          Reflection::call_delete(key.value());
        }
      }
    );
  }

 private:
  static ExpectedValue cast_float(void* pointer) {
    auto value = *reinterpret_cast<float*>(pointer);
    return value;
  }

  static ExpectedValue cast_double(void* pointer) {
    auto value = *reinterpret_cast<double*>(pointer);
    return value;
  }

  static ExpectedValue cast_int8_t(void* pointer) {
    auto value = *reinterpret_cast<int8_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint8_t(void* pointer) {
    auto value = *reinterpret_cast<uint8_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_int16_t(void* pointer) {
    auto value = *reinterpret_cast<int16_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint16_t(void* pointer) {
    auto value = *reinterpret_cast<uint16_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_int32_t(void* pointer) {
    auto value = *reinterpret_cast<int32_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint32_t(void* pointer) {
    auto value = *reinterpret_cast<uint32_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_int64_t(void* pointer) {
    auto value = *reinterpret_cast<int64_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_uint64_t(void* pointer) {
    auto value = *reinterpret_cast<uint64_t*>(pointer);
    return value;
  }

  static ExpectedValue cast_string(void* pointer) {
    auto* str_p = reinterpret_cast<std::string*>(pointer);

    return std::string_view(str_p->c_str());
  }

  static ExpectedValue cast_cstring(void* pointer) {

    return std::string_view(reinterpret_cast<const char*>(pointer));
  }

  static ExpectedValue cast_string_view(void* pointer) {
    auto value = *reinterpret_cast<std::string_view*>(pointer);
    return value;
  }

 public:
  static constexpr inline ExpectedValue (*kFunctions[13])(void*) = {
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