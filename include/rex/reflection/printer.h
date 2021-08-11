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
      [result](const Bool& b) { *result += b.to_string(); },     //
      [result](const Integer& i) { *result += i.to_string(); },  //
      [result](const Float& f) { *result += f.to_string(); },
      [result](const String& s) {
        *result += "'";
        *result += s.get();
        *result += "'";
      },
      [result](const Enum& e) { *result += e.to_string(); },
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
  std::cout << sprint(info) << std::endl;
}

void print(Var var) {
  print(reflect(var));
}

template <typename T>
void print(const T* pointer) {
  print(reflect(pointer));
}

}  // namespace Reflection

}  // namespace rr