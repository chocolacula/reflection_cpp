#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

template <typename T>
struct FloatActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Float(static_cast<double*>(value), sizeof(T), is_const));
  }
};

template <typename T>
typename std::enable_if_t<std::is_floating_point_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&FloatActions<T>::reflect,         //
                                                 &CommonActions<T>::type_name,      //
                                                 &CommonActions<T>::type_size,      //
                                                 &CommonActions<T>::alloc_default,  //
                                                 &CommonActions<T>::call_delete,    //
                                                 &CommonActions<T>::copy,           //
                                                 &CommonActions<T>::copy_default)));
  return id;
}

}  // namespace rr