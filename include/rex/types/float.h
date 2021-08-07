#pragma once

#include "../variable/cell.h"
#include "helper.h"
#include "rex/reflection/the_great_table.h"

namespace rr {

template <>
struct TypeHelper<float> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Primitive(Cell(static_cast<float*>(value), is_const)));
  }

  static std::string_view type_name() {
    return "float";
  }

  static size_t type_size() {
    return sizeof(float);
  }

  static Var alloc_default() {
    auto* p = new float(0.0);
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<float*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<float*>(to) = *static_cast<const float*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<float*>(to) = float(0.0);
    return true;
  }
};

template <>
TypeId TypeId::get(float* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<float>::reflect,        //
                                                 &TypeHelper<float>::type_name,      //
                                                 &TypeHelper<float>::type_size,      //
                                                 &TypeHelper<float>::alloc_default,  //
                                                 &TypeHelper<float>::call_delete,    //
                                                 &TypeHelper<float>::copy,           //
                                                 &TypeHelper<float>::copy_default)));
  return id;
}

}  // namespace rr