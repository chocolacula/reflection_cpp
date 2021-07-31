#pragma once

#include <vector>

#include "rex/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::vector<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::vector<T>*>(value)));
  }

  static std::string_view type_name() {
    static auto name = fmt::format("std::vector<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::vector<T>);
  }

  static Var alloc_default() {
    auto* p = new std::vector<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::vector<T>*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<std::vector<T>*>(from) = *static_cast<std::vector<T>*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::vector<T>*>(to) = std::vector<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::vector<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::vector<T>>::reflect,        //
                                                 &TypeHelper<std::vector<T>>::type_name,      //
                                                 &TypeHelper<std::vector<T>>::type_size,      //
                                                 &TypeHelper<std::vector<T>>::alloc_default,  //
                                                 &TypeHelper<std::vector<T>>::call_delete,    //
                                                 &TypeHelper<std::vector<T>>::copy,           //
                                                 &TypeHelper<std::vector<T>>::copy_default)));
  return id;
}

}  // namespace rr