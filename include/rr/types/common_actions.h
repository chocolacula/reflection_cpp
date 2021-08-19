#pragma once

#include "../reflection/the_great_table.h"
#include "../tools/names.h"

namespace rr {

template <typename T>
struct CommonActions {
  static std::string_view type_name() {
    static auto name = Names<T>::get();
    return name;
  }

  static size_t type_size() {
    return sizeof(T);
  }

  static Var alloc_default() {
    auto* p = new T();
    return Var(p);
  }

  static void call_delete(void* pointer) {
    delete static_cast<T*>(pointer);
  }

  static void copy(void* to, const void* from) {
    *static_cast<T*>(to) = *static_cast<const T*>(from);
  }

  static bool copy_default(void* to, size_t size) {
    if (size < type_size()) {
      return false;
    }

    *static_cast<T*>(to) = T();
    return true;
  }
};

}  // namespace rr