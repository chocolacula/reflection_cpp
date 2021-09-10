#pragma once

#include "rr/constexpr_map.h"
#include "rr/expected.h"
#include "rr/reflection/the_great_table.h"
#include "rr/types/common_actions.h"
#include "rr/types/type_actions.h"

// injected
#include "../../data/color.h"

namespace rr {

template <>
struct TypeActions<Colors> {

  static TypeInfo reflect(void* value, bool is_const) {

    static constexpr ConstexprMap<Colors, 6> constants = {{{
        {Colors::kWhite, "kWhite"},  //
        {Colors::kRed, "kRed"},      //
        {Colors::kGreen, "kGreen"},  //
        {Colors::kBlue, "kBlue"},    //
        {Colors::kBlack, "kBlack"},  //
        {Colors::kGray, "kGray"},
    }}};

    return Enum(static_cast<Colors*>(value), is_const, constants);
  }
};

template <>
TypeId TypeId::get(Colors* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Colors>::reflect,        //
                                                 &CommonActions<Colors>::type_name,    //
                                                 &CommonActions<Colors>::type_size,    //
                                                 &CommonActions<Colors>::call_new,     //
                                                 &CommonActions<Colors>::call_delete,  //
                                                 &CommonActions<Colors>::copy)));
  return id;
}

}  // namespace rr
