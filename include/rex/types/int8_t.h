#pragma once

#include "helper.h"
#include "rex/the_great_table.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<int8_t> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Primitive(static_cast<int8_t*>(value)));
  }

  static std::string_view type_name() {
    return "int8_t";
  }

  static size_t type_size() {
    return sizeof(int8_t);
  }

  static Var alloc_default() {
    auto* p = new int8_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<int8_t*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<int8_t*>(from) = *static_cast<int8_t*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<int8_t*>(to) = int8_t(0);
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<int8_t*>(key_pointer));
  // }
};

template <>
TypeId TypeId::get(int8_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<int8_t>::reflect,        //
                                                 &TypeHelper<int8_t>::type_name,      //
                                                 &TypeHelper<int8_t>::type_size,      //
                                                 &TypeHelper<int8_t>::alloc_default,  //
                                                 &TypeHelper<int8_t>::call_delete,    //
                                                 &TypeHelper<int8_t>::copy,           //
                                                 &TypeHelper<int8_t>::copy_default)));
  return id;
}

}  // namespace rr