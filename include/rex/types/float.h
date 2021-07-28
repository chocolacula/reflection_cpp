#pragma once

#include "helper.h"
#include "rex/the_great_table.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<float> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Primitive(static_cast<float*>(value)));
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

  static void copy(void* from, void* to) {
    *static_cast<float*>(from) = *static_cast<float*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<float*>(to) = float(0.0);
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<float*>(key_pointer));
  // }
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