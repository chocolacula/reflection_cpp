#pragma once

#include "type_actions.h"
#include "type_actions_def.h"

namespace rr {

class Reflection {
 public:
  static TypeInfo reflect(Var variable) {
    return TheGreatTable::data()[variable.type().number()].reflect(const_cast<void*>(variable.raw()),
                                                                   variable.is_const());
  }

  template <typename T>
  static TypeInfo reflect(T* pointer) {
    return reflect(Var(pointer));
  }

  template <typename T>
  static TypeInfo reflect(const T* pointer) {
    return reflect(Var(pointer));
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
    TheGreatTable::data()[variable.type().number()].call_delete(variable.raw_mut());
  }

  static void copy(Var to, Var from) {
    TheGreatTable::data()[from.type().number()].copy(to.raw_mut(), from.raw());
  }

  static bool copy_default(TypeId id, void* to, size_t size) {
    return TheGreatTable::data()[id.number()].copy_default(to, size);
  }

 private:
  static void append(const TypeInfo& info, std::string* result) {
    info.match(
        [result](const Object& o) {
          for (auto&& record : o.get_all_fields()) {
            // add the field name and trailing whitespace
            *result += record.first;
            *result += ": ";

            auto field_info = reflect(record.second);
            append(field_info, result);
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
        [result](const Array& a) {
          *result += "[";

          a.for_each([result](Var entry) {
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
        [result](const Sequence& s) {
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
        [result](const Map& m) {
          *result += "[";

          m.for_each([result](Var key, Var value) {
            auto key_info = reflect(key);
            append(key_info, result);

            *result += ": ";

            auto value_info = reflect(value);
            append(value_info, result);

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