#pragma once

#include <cstddef>
#include <cstring>

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

  static void* call_new(void* place, size_t place_size) {
    if (place_size >= sizeof(T)) {
      new (place) T[size_v]{};
      return place;
    }
    auto* p = new T[size_v];
    return p;
  }

  static void call_delete(void* pointer, bool in_place) {
    if (!in_place) {
      delete[] static_cast<T*>(pointer);
    }
  }

  static void copy(void* to, const void* from) {
    std::memcpy(to, from, size_v);
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(T (*)[size_v]) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<T[size_v]>::reflect,      //
                                                 &TypeActions<T[size_v]>::type_name,    //
                                                 &TypeActions<T[size_v]>::type_size,    //
                                                 &TypeActions<T[size_v]>::call_new,     //
                                                 &TypeActions<T[size_v]>::call_delete,  //
                                                 &TypeActions<T[size_v]>::copy)));
  return id;
}

}  // namespace rr