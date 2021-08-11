#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

template <typename T>
struct FloatHelper {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Float(static_cast<double*>(value), sizeof(T), is_const));
  }
};

template <typename T>
typename std::enable_if_t<std::is_floating_point_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&FloatHelper<T>::reflect,         //
                                                 &CommonHelper<T>::type_name,      //
                                                 &CommonHelper<T>::type_size,      //
                                                 &CommonHelper<T>::alloc_default,  //
                                                 &CommonHelper<T>::call_delete,    //
                                                 &CommonHelper<T>::copy,           //
                                                 &CommonHelper<T>::copy_default)));
  return id;
}

}  // namespace rr