#pragma once

#include "rex/type_info/type_info.h"
#include "rex/var/var.h"

namespace rr {

struct Actions {

  constexpr Actions(TypeInfo (*reflect)(void*, bool),  //
                    std::string_view (*get_name)(),    //
                    size_t (*size)(),                  //
                    Var (*alloc_default)(),            //
                    void (*call_delete)(void*),        //
                    void (*copy)(void*, void*),        //
                    bool (*copy_default)(void*, size_t))
      : reflect(reflect),
        type_name(get_name),
        type_size(size),
        alloc_default(alloc_default),
        call_delete(call_delete),
        copy(copy),
        copy_default(copy_default) {
  }

  constexpr Actions(const Actions& other) = default;
  constexpr Actions& operator=(const Actions& other) = default;

  // there is no reason to move the struct

  TypeInfo (*reflect)(void*, bool);
  std::string_view (*type_name)();
  size_t (*type_size)();
  Var (*alloc_default)();
  void (*call_delete)(void*);
  void (*copy)(void*, void*);
  bool (*copy_default)(void*, size_t);
};

}  // namespace rr
