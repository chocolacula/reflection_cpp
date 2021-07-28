#pragma once

#include "helper.h"
#include "rex/the_great_table.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<char> {

  static TypeInfo reflect(void* value) {
    return TypeInfo(Primitive(static_cast<char*>(value)));
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

  static void copy(void* from, void* to) {
    *static_cast<char*>(from) = *static_cast<char*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<char*>(to) = char(0);
    return true;
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<char*>(key_pointer));
  // }
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