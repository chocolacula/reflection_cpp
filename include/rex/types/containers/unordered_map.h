#pragma once

#include <unordered_map>

#include "rex/reflection/the_great_table.h"
#include "rex/types/helper.h"

namespace rr {

template <typename KeyT, typename ValueT>
struct TypeHelper<std::unordered_map<KeyT, ValueT>> {

  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Map(static_cast<std::unordered_map<KeyT, ValueT>*>(value), is_const));
  }

  static std::string_view type_name() {
    static auto name =
        format("std::unordered_map<{}, {}>", TypeHelper<KeyT>::type_name(), TypeHelper<ValueT>::type_name());
    return name;
  }

  static size_t type_size() {
    return sizeof(std::unordered_map<KeyT, ValueT>);
  }

  static Var alloc_default() {
    auto* p = new std::unordered_map<KeyT, ValueT>();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<std::unordered_map<KeyT, ValueT>*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<std::unordered_map<KeyT, ValueT>*>(to) = *static_cast<const std::unordered_map<KeyT, ValueT>*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<std::unordered_map<KeyT, ValueT>*>(to) = std::unordered_map<KeyT, ValueT>();
    return true;
  }
};

template <typename KeyT, typename ValueT>
TypeId TypeId::get(std::unordered_map<KeyT, ValueT>* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<std::unordered_map<KeyT, ValueT>>::reflect,        //
                                                 &TypeHelper<std::unordered_map<KeyT, ValueT>>::type_name,      //
                                                 &TypeHelper<std::unordered_map<KeyT, ValueT>>::type_size,      //
                                                 &TypeHelper<std::unordered_map<KeyT, ValueT>>::alloc_default,  //
                                                 &TypeHelper<std::unordered_map<KeyT, ValueT>>::call_delete,    //
                                                 &TypeHelper<std::unordered_map<KeyT, ValueT>>::copy,           //
                                                 &TypeHelper<std::unordered_map<KeyT, ValueT>>::copy_default)));
  return id;
}

}  // namespace rr