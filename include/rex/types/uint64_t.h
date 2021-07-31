#pragma once

#include "../the_great_table.h"
#include "../var/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<uint64_t> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<uint64_t*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "uint64_t";
  }

  static size_t type_size() {
    return sizeof(uint64_t);
  }

  static Var alloc_default() {
    auto* p = new uint64_t(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<uint64_t*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<uint64_t*>(from) = *static_cast<uint64_t*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<uint64_t*>(to) = uint64_t(0);
    return true;
  }
};

template <>
TypeId TypeId::get(uint64_t* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<uint64_t>::reflect,        //
                                                 &TypeHelper<uint64_t>::type_name,      //
                                                 &TypeHelper<uint64_t>::type_size,      //
                                                 &TypeHelper<uint64_t>::alloc_default,  //
                                                 &TypeHelper<uint64_t>::call_delete,    //
                                                 &TypeHelper<uint64_t>::copy,           //
                                                 &TypeHelper<uint64_t>::copy_default)));
  return id;
}

}  // namespace rr