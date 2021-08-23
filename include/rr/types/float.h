#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

template <typename T>
struct FloatActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Float(static_cast<double*>(value), sizeof(T), is_const));
  }

  static void call_delete(void* pointer, bool in_place) {
    if (!in_place) {
      delete static_cast<T*>(pointer);
    }
  }
};

template <typename T>
typename std::enable_if_t<std::is_floating_point_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&FloatActions<T>::reflect,     //
                                                 &CommonActions<T>::type_name,  //
                                                 &CommonActions<T>::type_size,  //
                                                 &CommonActions<T>::call_new,   //
                                                 &FloatActions<T>::call_delete)));
  return id;
}

}  // namespace rr