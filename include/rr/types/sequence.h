#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

template <typename T>
struct SequenceActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<T*>(value), is_const));
  }
};

template <typename T>
typename std::enable_if_t<is_sequence_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&SequenceActions<T>::reflect,    //
                                                 &CommonActions<T>::type_name,    //
                                                 &CommonActions<T>::type_size,    //
                                                 &CommonActions<T>::call_new,     //
                                                 &CommonActions<T>::call_delete,  //
                                                 &CommonActions<T>::copy)));
  return id;
}

}  // namespace rr