#pragma once

#include "../variable/cell.h"
#include "helper.h"
#include "rex/reflection/the_great_table.h"

namespace rr {

template <>
struct TypeHelper<uint16_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<uint16_t*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "uint16_t";
  }

  static size_t type_size() {
    return sizeof(uint16_t);
  }

  static Var alloc_default() {
    auto* p = new uint16_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<uint16_t*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<uint16_t*>(to) = *static_cast<const uint16_t*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<uint16_t*>(to) = uint16_t(0);
    return true;
  }
};

template <>
TypeId TypeId::get(uint16_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<uint16_t>::reflect,        //
                                                 &TypeHelper<uint16_t>::type_name,      //
                                                 &TypeHelper<uint16_t>::type_size,      //
                                                 &TypeHelper<uint16_t>::alloc_default,  //
                                                 &TypeHelper<uint16_t>::call_delete,    //
                                                 &TypeHelper<uint16_t>::copy,           //
                                                 &TypeHelper<uint16_t>::copy_default)));
  return id;
}

}  // namespace rr