#pragma once

#include "rr/reflection/the_great_table.h"
#include "rr/types/common_actions.h"
#include "rr/types/type_actions.h"

// injected
#include "../../config.h"

namespace rr {

template <>
struct TypeActions<Config> {

  static TypeInfo reflect(void* value, bool is_const) {
    auto p = static_cast<Config*>(value);

    // clang-format off
    return Object(Var(p, is_const), {
      {"templates", Var(&p->templates)},
      {"root_dir", Var(&p->root_dir)},
      {"input", Var(&p->input)},
      {"output_dir", Var(&p->output_dir)},
    });
    // clang-format on
  }
};

template <>
TypeId TypeId::get(Config* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Config>::reflect,      //
                                                 &CommonActions<Config>::type_name,  //
                                                 &CommonActions<Config>::type_size,  //
                                                 &CommonActions<Config>::call_new,   //
                                                 &CommonActions<Config>::call_delete)));
  return id;
}

}  // namespace rr
