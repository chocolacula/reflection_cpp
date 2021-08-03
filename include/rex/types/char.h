#pragma once

#include "../the_great_table.h"
#include "../variable/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<char> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<char*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "char";
  }

  static size_t type_size() {
    return sizeof(char);
  }

  static Var alloc_default() {
    auto* p = new char(0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<char*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<char*>(to) = *static_cast<const char*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<char*>(to) = char(0);
    return true;
  }
};

template <>
TypeId TypeId::get(char* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<char>::reflect,        //
                                                 &TypeHelper<char>::type_name,      //
                                                 &TypeHelper<char>::type_size,      //
                                                 &TypeHelper<char>::alloc_default,  //
                                                 &TypeHelper<char>::call_delete,    //
                                                 &TypeHelper<char>::copy,           //
                                                 &TypeHelper<char>::copy_default)));
  return id;
}

}  // namespace rr