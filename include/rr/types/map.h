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
  static TypeId id(TheGreatTable::record(Actions(&MapActions<T>::reflect,         //
                                                 &CommonActions<T>::type_name,    //
                                                 &CommonActions<T>::type_size,    //
                                                 &CommonActions<T>::call_new,     //
                                                 &CommonActions<T>::call_delete,  //
                                                 &CommonActions<T>::copy)));
  return id;
}

}  // namespace rr