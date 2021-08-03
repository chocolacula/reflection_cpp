#pragma once

#include "../the_great_table.h"
#include "../variable/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<int64_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<int64_t*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "int64_t";
  }

  static size_t type_size() {
    return sizeof(int64_t);
  }

  static Var alloc_default() {
    auto* p = new int64_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<int64_t*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<int64_t*>(to) = *static_cast<const int64_t*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<int64_t*>(to) = int64_t(0);
    return true;
  }
};

template <>
TypeId TypeId::get(int64_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<int64_t>::reflect,        //
                                                 &TypeHelper<int64_t>::type_name,      //
                                                 &TypeHelper<int64_t>::type_size,      //
                                                 &TypeHelper<int64_t>::alloc_default,  //
                                                 &TypeHelper<int64_t>::call_delete,    //
                                                 &TypeHelper<int64_t>::copy,           //
                                                 &TypeHelper<int64_t>::copy_default)));
  return id;
}

}  // namespace rr