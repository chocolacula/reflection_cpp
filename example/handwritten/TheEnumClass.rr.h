#pragma once

#include <frozen/unordered_map.h>

#include <string_view>

#include "rex/expected/error.h"
#include "rex/expected/expected.h"
#include "rex/info/enum_info.h"

// from input parameter
#include "../enum.h"

namespace rr {

template <>
class EnumInfo<TheEnumClass> {
 public:
  static Expected<std::string_view, Error> to_string(const TheEnumClass constant) {
    if (kConstants.count(constant) != 0)
      return kConstants.at(constant);

    return Error("Cannot find the constant");
  }

  static Expected<TheEnumClass, Error> parse(std::string_view name) {

    for (auto&& item : kConstants) {
      if (item.second == name)
        return item.first;
    }

    return Error(std::move("Cannot find the constant '" + std::string(name) + "'"));
  }

 private:
  static constexpr frozen::unordered_map<TheEnumClass, std::string_view, 5> kConstants = {
      {TheEnumClass::kWhite, "kWhite"},  //
      {TheEnumClass::kRed, "kRed"},      //
      {TheEnumClass::kGreen, "kGreen"},  //
      {TheEnumClass::kBlue, "kBlue"},    //
      {TheEnumClass::kBlack, "kBlack"},
  };
};

}  // namespace rr