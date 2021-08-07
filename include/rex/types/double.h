#pragma once

#include "../variable/cell.h"
#include "helper.h"
#include "rex/reflection/the_great_table.h"

namespace rr {

template <>
struct TypeHelper<double> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<double*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "double";
  }

  static size_t type_size() {
    return sizeof(double);
  }

  static Var alloc_default() {
    auto* p = new double(0.0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<double*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<double*>(to) = *static_cast<const double*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<double*>(to) = double(0.0);
    return true;
  }
};

template <>
TypeId TypeId::get(double* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<double>::reflect,        //
                                                 &TypeHelper<double>::type_name,      //
                                                 &TypeHelper<double>::type_size,      //
                                                 &TypeHelper<double>::alloc_default,  //
                                                 &TypeHelper<double>::call_delete,    //
                                                 &TypeHelper<double>::copy, &TypeHelper<double>::copy_default)));
  return id;
}

}  // namespace rr