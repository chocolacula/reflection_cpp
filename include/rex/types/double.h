#pragma once

#include "helper.h"
#include "rex/the_great_table.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<double> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Primitive(static_cast<double*>(value)));
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

  static void copy(void* from, void* to) {
    *static_cast<double*>(from) = *static_cast<double*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<double*>(to) = double(0.0);
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<double*>(key_pointer));
  // }
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