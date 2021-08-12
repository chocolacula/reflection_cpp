#pragma once

#include <string_view>

#include "rex/constexpr_map.h"
#include "rex/expected.h"
#include "rex/types/type_actions.h"

// from input parameter
#include "../../enum.h"

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
  static TypeId id(TheGreatTable::record(Actions(&TypeActions<Colors>::reflect,          //
                                                 &CommonActions<Colors>::type_name,      //
                                                 &CommonActions<Colors>::type_size,      //
                                                 &CommonActions<Colors>::alloc_default,  //
                                                 &CommonActions<Colors>::call_delete,    //
                                                 &CommonActions<Colors>::copy,           //
                                                 &CommonActions<Colors>::copy_default)));
  return id;
}

}  // namespace rr
