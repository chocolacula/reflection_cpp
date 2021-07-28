#pragma once

#include "helper.h"
#include "rex/the_great_table.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<int32_t> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Primitive(static_cast<int32_t*>(value)));
  }

  static std::string_view type_name() {
    return "int32_t";
  }

  static size_t type_size() {
    return sizeof(int32_t);
  }

  static Var alloc_default() {
    auto* p = new int32_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<int32_t*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<int32_t*>(from) = *static_cast<int32_t*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<int32_t*>(to) = int32_t(0);
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<int32_t*>(key_pointer));
  // }
};

template <>
TypeId TypeId::get(int32_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<int32_t>::reflect,        //
                                                 &TypeHelper<int32_t>::type_name,      //
                                                 &TypeHelper<int32_t>::type_size,      //
                                                 &TypeHelper<int32_t>::alloc_default,  //
                                                 &TypeHelper<int32_t>::call_delete,    //
                                                 &TypeHelper<int32_t>::copy,           //
                                                 &TypeHelper<int32_t>::copy_default)));
  return id;
}

}  // namespace rr