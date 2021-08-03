#pragma once

#include <set>

#include "rex/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::set<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::set<T>*>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name = fmt::format("std::set<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::set<T>);
  }

  static Var alloc_default() {
    auto* p = new std::set<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::set<T>*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::set<T>*>(to) = *static_cast<const std::set<T>*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::set<T>*>(to) = std::set<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::set<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::set<T>>::reflect,        //
                                                 &TypeHelper<std::set<T>>::type_name,      //
                                                 &TypeHelper<std::set<T>>::type_size,      //
                                                 &TypeHelper<std::set<T>>::alloc_default,  //
                                                 &TypeHelper<std::set<T>>::call_delete,    //
                                                 &TypeHelper<std::set<T>>::copy,           //
                                                 &TypeHelper<std::set<T>>::copy_default)));
  return id;
}

}  // namespace rr