#pragma once

#include "helper.h"
#include "rex/the_great_table.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<std::string> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Primitive(static_cast<std::string*>(value)));
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

  static void copy(void* from, void* to) {
    *static_cast<std::string*>(from) = *static_cast<std::string*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::string*>(to) = std::string();
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<std::string*>(key_pointer));
  // }
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