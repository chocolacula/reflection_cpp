#pragma once

#include "rex/types/helper.h"

// from input parameter
#include "../../objects.h"

// #include "../info/values/dictionary.h"

namespace rr {

template <>
struct TypeHelper<TheStruct1> {

  static TypeInfo reflect(void* value) {
    auto p = static_cast<TheStruct1*>(value);

    return Object(Var(p), {{"host", Var(&p->host)},         //
                           {"server_port", Var(&p->port)},  //
                           {"error_codes", Var(&p->error_codes)}});
  }

  static std::string_view type_name() {
    return "TheStruct1";
  }

  static size_t type_size() {
    return sizeof(TheStruct1);
  }

  static Var alloc_default() {
    auto* p = new TheStruct1();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<TheStruct1*>(pointer);
  }

  static void copy(void* from, void* to) {
    *static_cast<TheStruct1*>(from) = *static_cast<TheStruct1*>(to);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<TheStruct1*>(to) = TheStruct1();
    return true;
  }
  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<TheStruct1*>(key_pointer));
  // }
};

template <>
TypeId TypeId::get(TheStruct1* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<TheStruct1>::reflect,        //
                                                 &TypeHelper<TheStruct1>::type_name,      //
                                                 &TypeHelper<TheStruct1>::type_size,      //
                                                 &TypeHelper<TheStruct1>::alloc_default,  //
                                                 &TypeHelper<TheStruct1>::call_delete,    //
                                                 &TypeHelper<TheStruct1>::copy,           //
                                                 &TypeHelper<TheStruct1>::copy_default)));
  return id;
}

}  // namespace rr
