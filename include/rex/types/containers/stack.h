#pragma once

#include <stack>

#include "rex/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::stack<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::stack<T>*>(value)));
  }

  static std::string_view type_name() {
    static auto name = fmt::format("std::stack<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::stack<T>);
  }

  static Var alloc_default() {
    auto* p = new std::stack<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::stack<T>*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<std::stack<T>*>(from) = *static_cast<std::stack<T>*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::stack<T>*>(to) = std::stack<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::stack<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::stack<T>>::reflect,        //
                                                 &TypeHelper<std::stack<T>>::type_name,      //
                                                 &TypeHelper<std::stack<T>>::type_size,      //
                                                 &TypeHelper<std::stack<T>>::alloc_default,  //
                                                 &TypeHelper<std::stack<T>>::call_delete,    //
                                                 &TypeHelper<std::stack<T>>::copy,           //
                                                 &TypeHelper<std::stack<T>>::copy_default)));
  return id;
}

}  // namespace rr