#pragma once

#include "../reflection/the_great_table.h"
#include "common_actions.h"

namespace rr {

struct BoolActions {
  static TypeInfo reflect(void* value, bool is_const) {
    return TypeInfo(Bool(static_cast<bool*>(value), is_const));
  }

  static void call_delete(void* pointer, bool in_place) {
    if (!in_place) {
      delete static_cast<bool*>(pointer);
    }
  }
};

template <>
TypeId TypeId::get(bool* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&BoolActions::reflect,            //
                                                 &CommonActions<bool>::type_name,  //
                                                 &CommonActions<bool>::type_size,  //
                                                 &CommonActions<bool>::call_new,   //
                                                 &BoolActions::call_delete)));
  return id;
}

}  // namespace rr