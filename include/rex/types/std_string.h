#pragma once

#include "../variable/cell.h"
#include "helper.h"
#include "rex/reflection/the_great_table.h"

namespace rr {

template <>
struct TypeHelper<std::string> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<std::string*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "std::string";
  }

  static size_t type_size() {
    return sizeof(std::string);
  }

  static Var alloc_default() {
    auto* p = new std::string();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::string*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::string*>(to) = *static_cast<const std::string*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::string*>(to) = std::string();
    return true;
  }
};

template <>
TypeId TypeId::get(std::string* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::string>::reflect,        //
                                                 &TypeHelper<std::string>::type_name,      //
                                                 &TypeHelper<std::string>::type_size,      //
                                                 &TypeHelper<std::string>::alloc_default,  //
                                                 &TypeHelper<std::string>::call_delete,    //
                                                 &TypeHelper<std::string>::copy,           //
                                                 &TypeHelper<std::string>::copy_default)));
  return id;
}

}  // namespace rr