#pragma once

#include "../variable/cell.h"
#include "helper.h"
#include "rex/reflection/the_great_table.h"

namespace rr {

template <>
struct TypeHelper<uint8_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<uint8_t*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "uint8_t";
  }

  static size_t type_size() {
    return sizeof(uint8_t);
  }

  static Var alloc_default() {
    auto* p = new uint8_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<uint8_t*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<uint8_t*>(to) = *static_cast<const uint8_t*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<uint8_t*>(to) = uint8_t(0);
    return true;
  }
};

template <>
TypeId TypeId::get(uint8_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<uint8_t>::reflect,        //
                                                 &TypeHelper<uint8_t>::type_name,      //
                                                 &TypeHelper<uint8_t>::type_size,      //
                                                 &TypeHelper<uint8_t>::alloc_default,  //
                                                 &TypeHelper<uint8_t>::call_delete,    //
                                                 &TypeHelper<uint8_t>::copy,           //
                                                 &TypeHelper<uint8_t>::copy_default)));
  return id;
}

}  // namespace rr