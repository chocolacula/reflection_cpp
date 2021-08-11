#pragma once

#include <array>

#include "../../reflection/the_great_table.h"
#include "../helper.h"

namespace rr {

template <typename T, size_t size_v>
struct TypeHelper<std::array<T, size_v>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Array(static_cast<std::array<T, size_v>*>(value), is_const));
  }

  static bool copy_default(void* to, size_t size) {
    if (size < CommonHelper<std::array<T, size_v>>::type_size()) {
      return false;
    }
    // do nothing array is already on the stack in 'memory'
    return true;
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(std::array<T, size_v>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::array<T, size_v>>::reflect,          //
                                                 &CommonHelper<std::array<T, size_v>>::type_name,      //
                                                 &CommonHelper<std::array<T, size_v>>::type_size,      //
                                                 &CommonHelper<std::array<T, size_v>>::alloc_default,  //
                                                 &CommonHelper<std::array<T, size_v>>::call_delete,    //
                                                 &CommonHelper<std::array<T, size_v>>::copy,           //
                                                 &TypeHelper<std::array<T, size_v>>::copy_default)));
  return id;
}

}  // namespace rr