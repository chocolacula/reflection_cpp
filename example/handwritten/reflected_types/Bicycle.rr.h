#pragma once

#include "rr/reflection/the_great_table.h"
#include "rr/types/common_actions.h"
#include "rr/types/type_actions.h"

// injected
#include "../../data/bicycle.h"

namespace rr {

template <>
struct TypeActions<Bicycle> {

  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Bicycle*>(value);

    return Object(Var(p, is_const), {{"id", Var(&p->id)},                       //
                                     {"is_hardtail", Var(&p->is_hardtail)},     //
                                     {"manufacturer", Var(&p->manufacturer)},   //
                                     {"model", Var(&p->model)},                 //
                                     {"weight", Var(&p->frame_weight)},         //
                                     {"wheel_size", Var(&p->wheel_size_inch)},  //
                                     {"colors", Var(&p->colors)}});
  }
};

template <>
TypeId TypeId::get(Bicycle* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Bicycle>::reflect,        //
                                                 &CommonActions<Bicycle>::type_name,    //
                                                 &CommonActions<Bicycle>::type_size,    //
                                                 &CommonActions<Bicycle>::call_new,     //
                                                 &CommonActions<Bicycle>::call_delete,  //
                                                 &CommonActions<Bicycle>::copy)));
  return id;
}

}  // namespace rr
