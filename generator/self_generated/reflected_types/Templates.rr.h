#pragma once

#include "rr/reflection/the_great_table.h"
#include "rr/types/common_actions.h"
#include "rr/types/type_actions.h"

// injected
#include "../../config.h"

namespace rr {

template <>
struct TypeActions<Config::Templates> {

  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Config::Templates*>(value);

    // clang-format off
    return Object(Var(p, is_const), {
      {"enum", FieldInfo(&p->for_enum, Access::kPublic)},
      {"object", FieldInfo(&p->object, Access::kPublic)},
    });
    // clang-format on
  }
};

template <>
TypeId TypeId::get(Config::Templates* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Config::Templates>::reflect,        //
                                                 &CommonActions<Config::Templates>::type_name,    //
                                                 &CommonActions<Config::Templates>::type_size,    //
                                                 &CommonActions<Config::Templates>::call_new,     //
                                                 &CommonActions<Config::Templates>::call_delete,  //
                                                 &CommonActions<Config::Templates>::copy)));
  return id;
}

}  // namespace rr
