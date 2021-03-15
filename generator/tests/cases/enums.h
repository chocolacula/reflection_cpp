#pragma once

#include "rex/attributes.h"

// clang-format off
enum class EnumClass0 {};

RR_ENUM
enum/*stupid comment*/class/*stupid comment*/EnumClass1/*stupid comment*/{/*stupid comment*/White, Red, Green, Blue, Black,};

RR_ENUM
enum class EnumClass2{ White, Red, Green, Blue, Black };

RR_ENUM
enum class EnumClass3 { // comment
  White, Red, Green,    // another one
  // it is not works in enums
  RR_EXCLUDE
  Blue
};

RR_ENUM
enum class EnumClass4
{
  RR_NAME(white)
  White,

  RR_NAME("red")
  Red,

  RR_NAME(green)
  Green,

  RR_NAME("blue")
  Blue,

  RR_NAME(black)
  Black
};

RR_ENUM
enum class EnumClass5{
  RR_NAME(white)
  RR_NAME("white_2")
  White
};

RR_ENUM
enum class EnumClass6{};

RR_ENUM
enum TheEnum { White, Red, Green, Blue, Black };