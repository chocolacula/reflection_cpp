#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

template <typename T>
struct MapActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Map(static_cast<T*>(value), is_const));
  }
};

template <typename T>
typename std::enable_if_t<is_map_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&MapActions<T>::reflect,           //
                                                 &CommonHelper<T>::type_name,      //
                                                 &CommonHelper<T>::type_size,      //
                                                 &CommonHelper<T>::alloc_default,  //
                                                 &CommonHelper<T>::call_delete,    //
                                                 &CommonHelper<T>::copy,           //
                                                 &CommonHelper<T>::copy_default)));
  return id;
}

}  // namespace rr