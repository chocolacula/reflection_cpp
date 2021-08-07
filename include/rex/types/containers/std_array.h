#pragma once

#include <array>

#include "rex/reflection/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename T, size_t size_v>
struct TypeHelper<std::array<T, size_v>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Array(static_cast<std::array<T, size_v>*>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name = format("std::array<{}, {}>", TypeHelper<T>::type_name(), size_v);
    return name;
  }

  static size_t type_size() {
    return sizeof(std::array<T, size_v>);
  }

  static Var alloc_default() {
    auto* p = new std::array<T, size_v>;
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::array<T, size_v>*>(pointer);
  }

  static void copy(void* to, const void* from) {
    std::memcpy(static_cast<std::array<T, size_v>*>(to)->data(),          //
                static_cast<const std::array<T, size_v>*>(from)->data(),  //
                size_v);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    // do nothing array is already on the stack in 'memory'
    return true;
  }
};

template <typename T, size_t size_v>
TypeId TypeId::get(std::array<T, size_v>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::array<T, size_v>>::reflect,        //
                                                 &TypeHelper<std::array<T, size_v>>::type_name,      //
                                                 &TypeHelper<std::array<T, size_v>>::type_size,      //
                                                 &TypeHelper<std::array<T, size_v>>::alloc_default,  //
                                                 &TypeHelper<std::array<T, size_v>>::call_delete,    //
                                                 &TypeHelper<std::array<T, size_v>>::copy,           //
                                                 &TypeHelper<std::array<T, size_v>>::copy_default)));
  return id;
}

}  // namespace rr