#pragma once

#include <list>

#include "rex/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct TypeHelper<std::list<T>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<std::list<T>*>(value)));
  }

  static std::string_view type_name() {
    static auto name = fmt::format("std::list<{}>", TypeHelper<T>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::list<T>);
  }

  static Var alloc_default() {
    auto* p = new std::list<T>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::list<T>*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<std::list<T>*>(from) = *static_cast<std::list<T>*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::list<T>*>(to) = std::list<T>();
    return true;
  }
};

template <typename T>
TypeId TypeId::get(std::list<T>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::list<T>>::reflect,        //
                                                 &TypeHelper<std::list<T>>::type_name,      //
                                                 &TypeHelper<std::list<T>>::type_size,      //
                                                 &TypeHelper<std::list<T>>::alloc_default,  //
                                                 &TypeHelper<std::list<T>>::call_delete,    //
                                                 &TypeHelper<std::list<T>>::copy,           //
                                                 &TypeHelper<std::list<T>>::copy_default)));
  return id;
}

}  // namespace rr