#pragma once

#include <frozen/unordered_map.h>
#include <string_view>

#include "../../src/expected/expectedString.h"
#include "../../src/info/enumInfo.h"
#include "../enum.h"

template <>
class EnumInfo<TheEnumClass> {
 public:
  static ExpectedString to_string(const TheEnumClass constant) {
    if (_constants.count(constant) != 0)
      return _constants.at(constant);

    return Error("Cannot find the constant");
  }

 private:
  static constexpr frozen::unordered_map<TheEnumClass, std::string_view, 5> _constants = {
    {TheEnumClass::White, "White"},
    {TheEnumClass::Red, "Red"},
    {TheEnumClass::Green, "Green"},
    {TheEnumClass::Blue, "Blue"},
    {TheEnumClass::Black, "Black"}};
};
