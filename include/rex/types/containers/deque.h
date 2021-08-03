#pragma once

#include <deque>

#include "rex/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::deque<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::deque<T>*>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name = fmt::format("std::deque<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::deque<T>);
  }

  static Var alloc_default() {
    auto* p = new std::deque<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::deque<T>*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::deque<T>*>(to) = *static_cast<const std::deque<T>*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::deque<T>*>(to) = std::deque<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::deque<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::deque<T>>::reflect,        //
                                                 &TypeHelper<std::deque<T>>::type_name,      //
                                                 &TypeHelper<std::deque<T>>::type_size,      //
                                                 &TypeHelper<std::deque<T>>::alloc_default,  //
                                                 &TypeHelper<std::deque<T>>::call_delete,    //
                                                 &TypeHelper<std::deque<T>>::copy,           //
                                                 &TypeHelper<std::deque<T>>::copy_default)));
  return id;
}

}  // namespace rr