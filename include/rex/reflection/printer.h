#pragma once

#include "../info/type_info/type_info.h"

namespace rr {

namespace Reflection {
TypeInfo reflect(Var var);

template <typename T>
TypeInfo reflect(T* pointer);

template <typename T>
TypeInfo reflect(const T* pointer);
}  // namespace Reflection

static void parse(const TypeInfo& info, std::string* result) {
  info.match(
      [result](const Object& o) {
        for (auto&& record : o.get_all_fields()) {
          // add the field name and trailing whitespace
          *result += record.first;
          *result += ": ";

          auto field_info = Reflection::reflect(record.second);
          parse(field_info, result);
          *result += '\n';
        }
      },
      [result](const Primitive& p) {
        p.match([result](Cell<bool> v) { *result += *v.ptr() ? "true" : "false"; },   //
                [result](Cell<float> v) { *result += std::to_string(*v.ptr()); },     //
                [result](Cell<double> v) { *result += std::to_string(*v.ptr()); },    //
                [result](Cell<int8_t> v) { *result += std::to_string(*v.ptr()); },    //
                [result](Cell<uint8_t> v) { *result += std::to_string(*v.ptr()); },   //
                [result](Cell<int16_t> v) { *result += std::to_string(*v.ptr()); },   //
                [result](Cell<uint16_t> v) { *result += std::to_string(*v.ptr()); },  //
                [result](Cell<int32_t> v) { *result += std::to_string(*v.ptr()); },   //
                [result](Cell<uint32_t> v) { *result += std::to_string(*v.ptr()); },  //
                [result](Cell<int64_t> v) { *result += std::to_string(*v.ptr()); },   //
                [result](Cell<uint64_t> v) { *result += std::to_string(*v.ptr()); },  //
                [result](Cell<std::string> v) {
                  *result += "'";
                  *result += *v.ptr();
                  *result += "'";
                },
                [result](Cell<std::string_view> v) {
                  *result += "'";
                  *result += *v.ptr();
                  *result += "'";
                },
                [](auto&&) {});
      },
      [result](const Map& m) {
        *result += "[";

        m.for_each([result](Var key, Var value) {
          auto key_info = Reflection::reflect(key);
          parse(key_info, result);

          *result += ": ";

          auto value_info = Reflection::reflect(value);
          parse(value_info, result);

          *result += ", ";
        });
        if ((*result)[result->size() - 2] == ',') {
          result->replace(result->size() - 2, 2, "]");
        } else {
          *result += "]";
        }
      },
      [result](const auto& as) {  // Array or Sequence
        *result += "[";

        as.for_each([result](Var entry) {
          auto entry_info = Reflection::reflect(entry);

          parse(entry_info, result);
          *result += ", ";
        });
        if ((*result)[result->size() - 2] == ',') {
          result->replace(result->size() - 2, 2, "]");
        } else {
          *result += "]";
        }
      });
}

namespace Reflection {

std::string sprint(const TypeInfo& info) {
  std::string result;
  parse(info, &result);
  return result;
}

std::string sprint(Var var) {
  return sprint(reflect(var));
}

template <typename T>
std::string sprint(const T* pointer) {
  return sprint(reflect(pointer));
}

void print(const TypeInfo& info) {
  std::string result;
  parse(info, &result);
  std::cout << result << std::endl;
}

void print(Var var) {
  sprint(reflect(var));
}

template <typename T>
void print(const T* pointer) {
  sprint(reflect(pointer));
}

}  // namespace Reflection

}  // namespace rr