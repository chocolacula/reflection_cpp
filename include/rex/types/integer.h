#pragma once

#include "common_actions.h"
#include "rex/reflection/the_great_table.h"

namespace rr {

template <typename T, bool is_signed>
struct IntHelper {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Integer(static_cast<uint64_t*>(value), sizeof(T), is_signed, is_const));
  }
};

template <typename T>
typename std::enable_if_t<std::is_integral_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&IntHelper<T, std::is_signed_v<T>>::reflect,  //
                                                 &CommonHelper<T>::type_name,                  //
                                                 &CommonHelper<T>::type_size,                  //
                                                 &CommonHelper<T>::alloc_default,              //
                                                 &CommonHelper<T>::call_delete,                //
                                                 &CommonHelper<T>::copy,                       //
                                                 &CommonHelper<T>::copy_default)));
  return id;
}

}  // namespace rr