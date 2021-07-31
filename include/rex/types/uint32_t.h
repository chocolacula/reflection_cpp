#pragma once

#include "../the_great_table.h"
#include "../var/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<uint32_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<uint32_t*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "uint32_t";
  }

  static size_t type_size() {
    return sizeof(uint32_t);
  }

  static Var alloc_default() {
    auto* p = new uint32_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<uint32_t*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<uint32_t*>(from) = *static_cast<uint32_t*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<uint32_t*>(to) = uint32_t(0);
    return true;
  }
};

template <>
TypeId TypeId::get(uint32_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<uint32_t>::reflect,        //
                                                 &TypeHelper<uint32_t>::type_name,      //
                                                 &TypeHelper<uint32_t>::type_size,      //
                                                 &TypeHelper<uint32_t>::alloc_default,  //
                                                 &TypeHelper<uint32_t>::call_delete,    //
                                                 &TypeHelper<uint32_t>::copy,           //
                                                 &TypeHelper<uint32_t>::copy_default)));
  return id;
}

}  // namespace rr