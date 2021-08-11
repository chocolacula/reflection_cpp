#pragma once

#include "rex/reflection/the_great_table.h"
#include "rex/tools.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T>
struct SequenceHelper {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Sequence(static_cast<T*>(value), is_const));
  }
};

template <typename T>
typename std::enable_if_t<is_sequence_v<T>, TypeId>  //
TypeId::get(T* ptr) {
  static TypeId id(TheGreatTable::record(Actions(&SequenceHelper<T>::reflect,      //
                                                 &CommonHelper<T>::type_name,      //
                                                 &CommonHelper<T>::type_size,      //
                                                 &CommonHelper<T>::alloc_default,  //
                                                 &CommonHelper<T>::call_delete,    //
                                                 &CommonHelper<T>::copy,           //
                                                 &CommonHelper<T>::copy_default)));
  return id;
}

}  // namespace rr