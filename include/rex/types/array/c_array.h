#pragma once

#include "../../reflection/the_great_table.h"
#include "../type_actions.h"

namespace rr {

template <typename T, size_t size_v>
struct TypeActions<T[size_v]> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Array(static_cast<T(*)[size_v]>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name = format("{}[{}]", TypeActions<T>::type_name(), size_v);
    return name;
  }

  static size_t type_size() {
    return sizeof(T[size_v]);
  }

  static Var alloc_default() {
    auto* p = new T[size_v];
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete[] static_cast<T*>(pointer);
  }

  static void copy(void* to, const void* from) {
    std::memcpy(static_cast<T*>(to), static_cast<const T*>(from), size_v);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }
    // do nothing array is already on the stack in 'memory'
    return true;
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(T (*/*unused*/)[size_v]) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<T[size_v]>::reflect,        //
                                                 &TypeActions<T[size_v]>::type_name,      //
                                                 &TypeActions<T[size_v]>::type_size,      //
                                                 &TypeActions<T[size_v]>::alloc_default,  //
                                                 &TypeActions<T[size_v]>::call_delete,    //
                                                 &TypeActions<T[size_v]>::copy,           //
                                                 &TypeActions<T[size_v]>::copy_default)));
  return id;
}

}  // namespace rr