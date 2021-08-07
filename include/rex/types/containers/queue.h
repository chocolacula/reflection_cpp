#pragma once

#include <queue>

#include "rex/reflection/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::queue<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::queue<T>*>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name = format("std::queue<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::queue<T>);
  }

  static Var alloc_default() {
    auto* p = new std::queue<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::queue<T>*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::queue<T>*>(to) = *static_cast<const std::queue<T>*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::queue<T>*>(to) = std::queue<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::queue<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::queue<T>>::reflect,        //
                                                 &TypeHelper<std::queue<T>>::type_name,      //
                                                 &TypeHelper<std::queue<T>>::type_size,      //
                                                 &TypeHelper<std::queue<T>>::alloc_default,  //
                                                 &TypeHelper<std::queue<T>>::call_delete,    //
                                                 &TypeHelper<std::queue<T>>::copy,           //
                                                 &TypeHelper<std::queue<T>>::copy_default)));
  return id;
}

}  // namespace rr