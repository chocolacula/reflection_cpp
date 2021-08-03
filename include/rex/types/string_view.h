#pragma once

#include "../the_great_table.h"
#include "../variable/cell.h"
#include "helper.h"

namespace rr {

template <>
struct TypeHelper<std::string_view> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<std::string_view*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "std::string_view";
  }

  static size_t type_size() {
    return sizeof(std::string_view);
  }

  static Var alloc_default() {
    auto* p = new std::string_view();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::string_view*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::string_view*>(to) = *static_cast<const std::string_view*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::string_view*>(to) = std::string_view();
    return true;
  }
};

template <>
TypeId TypeId::get(std::string_view* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::string_view>::reflect,        //
                                                 &TypeHelper<std::string_view>::type_name,      //
                                                 &TypeHelper<std::string_view>::type_size,      //
                                                 &TypeHelper<std::string_view>::alloc_default,  //
                                                 &TypeHelper<std::string_view>::call_delete,    //
                                                 &TypeHelper<std::string_view>::copy,           //
                                                 &TypeHelper<std::string_view>::copy_default)));
  return id;
}

}  // namespace rr