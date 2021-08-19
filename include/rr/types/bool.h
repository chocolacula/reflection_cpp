#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

struct BoolActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Bool(static_cast<bool*>(value), is_const));
  }
};

template <>
TypeId TypeId::get(bool* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&BoolActions::reflect,                //
                                                 &CommonActions<bool>::type_name,      //
                                                 &CommonActions<bool>::type_size,      //
                                                 &CommonActions<bool>::alloc_default,  //
                                                 &CommonActions<bool>::call_delete,    //
                                                 &CommonActions<bool>::copy,           //
                                                 &CommonActions<bool>::copy_default)));
  return id;
}

}  // namespace rr