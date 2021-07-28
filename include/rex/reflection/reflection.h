#pragma once

#include "type_actions.h"
#include "type_actions_def.h"

namespace rr {

class Reflection {
 public:
  static TypeInfo reflect(Var variable) {
    return TheGreatTable::data()[variable.type().number()].reflect(variable.value());
  }

  template <typename T>
  static TypeInfo reflect(T* pointer) {
    auto id = TypeId::get(pointer);
    Var var(pointer, id);

    return reflect(var);
  }

  static void print(TypeInfo info) {
    std::string result;

    append(info, &result);

    std::cout << result << std::endl;
  }

  static void print(Var var) {
    print(reflect(var));
  }

  template <typename T>
  static void print(T* pointer) {
    print(reflect(pointer));
  }

  static std::string_view type_name(TypeId id) {
    return TypeActions::type_name(id);
  }

  static size_t type_size(TypeId id) {
    return TypeActions::type_size(id);
  }

  static Var alloc_default(TypeId id) {
    return TheGreatTable::data()[id.number()].alloc_default();
  }

  static void call_delete(Var variable) {
    TheGreatTable::data()[variable.type().number()].call_delete(variable.value());
  }

  static void copy(Var from, Var to) {
    TheGreatTable::data()[from.type().number()].copy(from.value(), to.value());
  }

  static bool copy_default(TypeId id, void* to, size_t size) {
    return TheGreatTable::data()[id.number()].copy_default(to, size);
  }

 private:
  static void append(TypeInfo info, std::string* result) {
    info.match(
        [result](Object o) {
          for (auto&& record : o.get_all_fields()) {
            // add the field name and trailing whitespace
            *result += record.first;
            *result += ": ";

            auto field_info = reflect(record.second);
            append(field_info, result);
            *result += '\n';
          }
        },
        [result](Primitive p) {
          p.match([result](float* v) { *result += std::to_string(*v); },     //
                  [result](double* v) { *result += std::to_string(*v); },    //
                  [result](int8_t* v) { *result += std::to_string(*v); },    //
                  [result](uint8_t* v) { *result += std::to_string(*v); },   //
                  [result](int16_t* v) { *result += std::to_string(*v); },   //
                  [result](uint16_t* v) { *result += std::to_string(*v); },  //
                  [result](int32_t* v) { *result += std::to_string(*v); },   //
                  [result](uint32_t* v) { *result += std::to_string(*v); },  //
                  [result](int64_t* v) { *result += std::to_string(*v); },   //
                  [result](uint64_t* v) { *result += std::to_string(*v); },  //
                  [result](std::string* v) {
                    *result += "'";
                    *result += *v;
                    *result += "'";
                  },
                  [result](const char* v) {
                    *result += "'";
                    *result += *v;
                    *result += "'";
                  },
                  [result](std::string_view* v) {
                    *result += "'";
                    *result += *v;
                    *result += "'";
                  },
                  [](auto&&) {});
        },
        [result](Sequence s) {
          *result += "[";

          s.for_each([result](Var entry) {
            auto entry_info = reflect(entry);

            append(entry_info, result);
            *result += ", ";
          });
          if ((*result)[result->size() - 2] == ',') {
            result->replace(result->size() - 2, 2, "]");
          } else {
            *result += "]";
          }
        },
        [](auto&&) {});
  }
};

}  // namespace rr