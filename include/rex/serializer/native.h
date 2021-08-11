#pragma once

#include "../expected.h"
#include "../reflection/reflection.h"

namespace rr::json {

class Native {
 public:
  template <typename T>
  static Expected<T> from_string(std::string_view str) {
    // TODO implement
  }

  template <typename T>
  static Expected<std::string> to_string(const T& obj) {
    auto inf = Reflection::reflect(obj);

    std::string result;
    disassemble(inf, &result);
    return result;
  }

 private:
  static void disassemble(const TypeInfo& info, std::string* result) {
    info.match(
        // [result](const Object& o) {
        //   *result += '{';
        //   for (auto&& record : o.get_all_fields()) {
        //     *result += '"';
        //     *result += record.first;
        //     *result += "\":";
        //
        //     auto field_info = Reflection::reflect(record.second);
        //     disassemble(field_info, result);
        //     *result += ',';
        //   }
        //
        //   if ((*result)[result->size() - 1] == ',') {
        //     (*result)[result->size() - 1] = '}';
        //   } else {
        //     *result += '}';
        //   }
        // },
        // [result](const Primitive& p) {
        //   p.match([result](Cell<bool> v) { *result += *v.ptr() ? "true" : "false"; },   //
        //           [result](Cell<float> v) { *result += std::to_string(*v.ptr()); },     //
        //           [result](Cell<double> v) { *result += std::to_string(*v.ptr()); },    //
        //           [result](Cell<int8_t> v) { *result += std::to_string(*v.ptr()); },    //
        //           [result](Cell<uint8_t> v) { *result += std::to_string(*v.ptr()); },   //
        //           [result](Cell<int16_t> v) { *result += std::to_string(*v.ptr()); },   //
        //           [result](Cell<uint16_t> v) { *result += std::to_string(*v.ptr()); },  //
        //           [result](Cell<int32_t> v) { *result += std::to_string(*v.ptr()); },   //
        //           [result](Cell<uint32_t> v) { *result += std::to_string(*v.ptr()); },  //
        //           [result](Cell<int64_t> v) { *result += std::to_string(*v.ptr()); },   //
        //           [result](Cell<uint64_t> v) { *result += std::to_string(*v.ptr()); },  //
        //           [result](Cell<std::string> v) {
        //             *result += '"';
        //             *result += *v.ptr();
        //             *result += '"';
        //           },
        //           [result](Cell<std::string_view> v) {
        //             *result += '"';
        //             *result += *v.ptr();
        //             *result += '"';
        //           },
        //           [](auto&&) {});
        // },
        // [result](const Map& m) {
        //   *result += '{';
        //
        //   m.for_each([result](Var key, Var value) {
        //     auto key_info = Reflection::reflect(key);
        //     *result += "\"key\":";
        //     disassemble(key_info, result);
        //
        //     *result += ',';
        //
        //     auto value_info = Reflection::reflect(value);
        //     *result += "\"value\":";
        //     disassemble(value_info, result);
        //
        //     *result += ',';
        //   });
        //
        //   if ((*result)[result->size() - 1] == ',') {
        //     (*result)[result->size() - 1] = '}';
        //   } else {
        //     *result += '}';
        //   }
        // },
        [result](const auto& as) {  // Array or Sequence
          // *result += "[";
          //
          // as.for_each([result](Var entry) {
          //   auto entry_info = Reflection::reflect(entry);
          //
          //   disassemble(entry_info, result);
          //   *result += ",";
          // });
          //
          // if ((*result)[result->size() - 1] == ',') {
          //   (*result)[result->size() - 1] = ']';
          // } else {
          //   *result += ']';
          // }
        });
  }
};

}  // namespace rr::json