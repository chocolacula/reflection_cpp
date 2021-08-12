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
    auto info = Reflection::reflect(obj);

    std::string result;
    disassemble(info, &result);
    return result;
  }

 private:
  static void disassemble(const TypeInfo& info, std::string* result) {
    info.match(
        [result](const Object& o) {
          *result += '{';
          for (auto&& record : o.get_all_fields()) {
            *result += '"';
            *result += record.first;
            *result += "\":";

            auto field_info = Reflection::reflect(record.second);
            disassemble(field_info, result);
            *result += ',';
          }

          if ((*result)[result->size() - 1] == ',') {
            (*result)[result->size() - 1] = '}';
          } else {
            *result += '}';
          }
        },
        [result](const Bool& b) { *result += b.to_string(); },     //
        [result](const Integer& i) { *result += i.to_string(); },  //
        [result](const Float& f) { *result += f.to_string(); },
        [result](const String& s) {
          *result += '"';
          *result += s.get();
          *result += '"';
        },
        [result](const Enum& e) {
          *result += '"';
          *result += e.to_string();
          *result += '"';
        },
        [result](const Map& m) {
          *result += '{';

          m.for_each([result](Var key, Var value) {
            auto key_info = Reflection::reflect(key);
            *result += "\"key\":";
            disassemble(key_info, result);

            *result += ',';

            auto value_info = Reflection::reflect(value);
            *result += "\"value\":";
            disassemble(value_info, result);

            *result += ',';
          });

          if ((*result)[result->size() - 1] == ',') {
            (*result)[result->size() - 1] = '}';
          } else {
            *result += '}';
          }
        },
        [result](const auto& as) {  // Array or Sequence
          *result += "[";

          as.for_each([result](Var entry) {
            auto entry_info = Reflection::reflect(entry);

            disassemble(entry_info, result);
            *result += ",";
          });

          if ((*result)[result->size() - 1] == ',') {
            (*result)[result->size() - 1] = ']';
          } else {
            *result += ']';
          }
        });
  }
};

}  // namespace rr::json