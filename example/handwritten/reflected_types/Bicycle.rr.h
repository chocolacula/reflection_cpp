#pragma once

#include "rex/types/helper.h"

// from input parameter
#include "../../object.h"

namespace rr {

template <>
struct TypeHelper<Bicycle> {

  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Bicycle*>(value);

    return Object(Var(p, is_const), {{"id", Var(&p->id)},                      //
                                     {"manufacturer", Var(&p->manufacturer)},  //
                                     {"model", Var(&p->model)},
                                     {"weight", Var(&p->frame_weight)},
                                     {"wheel_size", Var(&p->wheel_size_inch)},
                                     {"colors", Var(&p->colors)}});
  }
};

template <>
TypeId TypeId::get(Bicycle* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<Bicycle>::reflect,          //
                                                 &CommonHelper<Bicycle>::type_name,      //
                                                 &CommonHelper<Bicycle>::type_size,      //
                                                 &CommonHelper<Bicycle>::alloc_default,  //
                                                 &CommonHelper<Bicycle>::call_delete,    //
                                                 &CommonHelper<Bicycle>::copy,           //
                                                 &CommonHelper<Bicycle>::copy_default)));
  return id;
}

}  // namespace rr
