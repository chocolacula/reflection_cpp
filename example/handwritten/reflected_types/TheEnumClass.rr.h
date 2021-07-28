#pragma once

#include <string_view>

#include "rex/constexpr_map.h"
#include "rex/expected.h"
#include "rex/info/enum_info.h"

// from input parameter
#include "../../enum.h"

namespace rr {

template <>
class EnumInfo<Colors> {
 public:
  static Expected<std::string_view> to_string(const Colors constant) {

    return kConstants.get_name(constant);
  }

  static Expected<Colors> parse(std::string_view name) {

    return kConstants.get_value(name);
  }

 private:
  static constexpr ConstexprMap<Colors, 5> kConstants = {{{
      {Colors::kWhite, "kWhite"},  //
      {Colors::kRed, "kRed"},      //
      {Colors::kGreen, "kGreen"},  //
      {Colors::kBlue, "kBlue"},    //
      {Colors::kBlack, "kBlack"},
  }}};
};

}  // namespace rr