#pragma once

#include <string_view>

#include "rex/constexpr_map.h"
#include "rex/expected.h"
#include "rex/types/helper.h"

// from input parameter
#include "../../enum.h"

namespace rr {

template <>
struct TypeHelper<Colors> {

  static TypeInfo reflect(void* value, bool is_const) {

    static constexpr ConstexprMap<Colors, 5> constants = {{{
        {Colors::kWhite, "kWhite"},  //
        {Colors::kRed, "kRed"},      //
        {Colors::kGreen, "kGreen"},  //
        {Colors::kBlue, "kBlue"},    //
        {Colors::kBlack, "kBlack"},
    }}};

    return Enum(static_cast<Colors*>(value), is_const, constants);
  }
};

template <>
TypeId TypeId::get(Colors* /*unused*/) {
  static TypeId id(TheGreatTable::record(Actions(&TypeHelper<Colors>::reflect,          //
                                                 &CommonHelper<Colors>::type_name,      //
                                                 &CommonHelper<Colors>::type_size,      //
                                                 &CommonHelper<Colors>::alloc_default,  //
                                                 &CommonHelper<Colors>::call_delete,    //
                                                 &CommonHelper<Colors>::copy,           //
                                                 &CommonHelper<Colors>::copy_default)));
  return id;
}

}  // namespace rr
