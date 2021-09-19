#pragma once

#include "rr/info/access.h"
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
      {"root_dir", FieldInfo(&p->root_dir, Access::kPublic)},
      {"compdb_dir", FieldInfo(&p->compdb_dir, Access::kPublic)},
      {"templates", FieldInfo(&p->templates, Access::kPublic)},
      {"input", FieldInfo(&p->input, Access::kPublic)},
      {"output_dir", FieldInfo(&p->output_dir, Access::kPublic)},
    });
    // clang-format on
  }
};

template <>
TypeId TypeId::get(Config* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Config>::reflect,        //
                                                 &CommonActions<Config>::type_name,    //
                                                 &CommonActions<Config>::type_size,    //
                                                 &CommonActions<Config>::call_new,     //
                                                 &CommonActions<Config>::call_delete,  //
                                                 &CommonActions<Config>::copy)));
  return id;
}

}  // namespace rr
