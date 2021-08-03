#pragma once

#include "../the_great_table.h"
#include "../variable/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<int32_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<int32_t*>(value), is_const)));
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

  static void copy(void* to, const void* from) {
    *static_cast<int32_t*>(to) = *static_cast<const int32_t*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<int32_t*>(to) = int32_t(0);
    return true;
  }
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