#pragma once

#include <unordered_set>

#include "rex/reflection/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::unordered_set<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::unordered_set<T>*>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name = format("std::unordered_set<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::unordered_set<T>);
  }

  static Var alloc_default() {
    auto* p = new std::unordered_set<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::unordered_set<T>*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::unordered_set<T>*>(to) = *static_cast<const std::unordered_set<T>*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::unordered_set<T>*>(to) = std::unordered_set<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::unordered_set<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::unordered_set<T>>::reflect,        //
                                                 &TypeHelper<std::unordered_set<T>>::type_name,      //
                                                 &TypeHelper<std::unordered_set<T>>::type_size,      //
                                                 &TypeHelper<std::unordered_set<T>>::alloc_default,  //
                                                 &TypeHelper<std::unordered_set<T>>::call_delete,    //
                                                 &TypeHelper<std::unordered_set<T>>::copy,           //
                                                 &TypeHelper<std::unordered_set<T>>::copy_default)));
  return id;
}

}  // namespace rr