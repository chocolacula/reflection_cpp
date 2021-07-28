#pragma once

#include "rex/the_great_table.h"
#include "rex/types/helper.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <typename T, size_t size_v>
struct TypeHelper<T[size_v]> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Sequence(static_cast<T(*)[size_v]>(value)));
  }

  static std::string_view type_name() {
    static auto name = fmt::format("{}[{}]", TypeHelper<T>::type_name(), size_v);
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

  static void copy(void* from, void* to) {
    std::memcpy(static_cast<T*>(from), static_cast<T*>(to), size_v);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    // do nothing array is already on the stack in 'memory'
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<std::vector<T>*>(key_pointer));
  // }
};

template <typename T, size_t size_v>
TypeId TypeId::get(T (*array)[size_v]) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<T[size_v]>::reflect,        //
                                                 &TypeHelper<T[size_v]>::type_name,      //
                                                 &TypeHelper<T[size_v]>::type_size,      //
                                                 &TypeHelper<T[size_v]>::alloc_default,  //
                                                 &TypeHelper<T[size_v]>::call_delete,    //
                                                 &TypeHelper<T[size_v]>::copy,           //
                                                 &TypeHelper<T[size_v]>::copy_default)));
  return id;
}

}  // namespace rr