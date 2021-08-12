#pragma once

#include <array>

#include "../../reflection/the_great_table.h"
#include "../type_actions.h"

namespace rr {

template <typename T, size_t size_v>
struct TypeActions<std::array<T, size_v>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Array(static_cast<std::array<T, size_v>*>(value), is_const));
  }

  static bool copy_default(void* to, size_t size) {
    if (size < CommonActions<std::array<T, size_v>>::type_size()) {
      return false;
    }
    // do nothing array is already on the stack in 'memory'
    return true;
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(std::array<T, size_v>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<std::array<T, size_v>>::reflect,          //
                                                 &CommonActions<std::array<T, size_v>>::type_name,      //
                                                 &CommonActions<std::array<T, size_v>>::type_size,      //
                                                 &CommonActions<std::array<T, size_v>>::alloc_default,  //
                                                 &CommonActions<std::array<T, size_v>>::call_delete,    //
                                                 &CommonActions<std::array<T, size_v>>::copy,           //
                                                 &TypeActions<std::array<T, size_v>>::copy_default)));
  return id;
}

}  // namespace rr