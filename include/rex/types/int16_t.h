#pragma once

#include "../the_great_table.h"
#include "../var/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<int16_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<int16_t*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "int16_t";
  }

  static size_t type_size() {
    return sizeof(int16_t);
  }

  static Var alloc_default() {
    auto* p = new int16_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<int16_t*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<int16_t*>(from) = *static_cast<int16_t*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<int16_t*>(to) = int16_t(0);
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<int16_t*>(key_pointer));
  // }
};

template <>
TypeId TypeId::get(int16_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<int16_t>::reflect,        //
                                                 &TypeHelper<int16_t>::type_name,      //
                                                 &TypeHelper<int16_t>::type_size,      //
                                                 &TypeHelper<int16_t>::alloc_default,  //
                                                 &TypeHelper<int16_t>::call_delete,    //
                                                 &TypeHelper<int16_t>::copy,           //
                                                 &TypeHelper<int16_t>::copy_default)));
  return id;
}

}  // namespace rr