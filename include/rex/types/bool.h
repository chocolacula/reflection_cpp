#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

struct BoolHelper {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Bool(static_cast<bool*>(value), is_const));
  }
};

template <>
TypeId TypeId::get(bool* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&BoolHelper::reflect,                //
                                                 &CommonHelper<bool>::type_name,      //
                                                 &CommonHelper<bool>::type_size,      //
                                                 &CommonHelper<bool>::alloc_default,  //
                                                 &CommonHelper<bool>::call_delete,    //
                                                 &CommonHelper<bool>::copy,           //
                                                 &CommonHelper<bool>::copy_default)));
  return id;
}

}  // namespace rr