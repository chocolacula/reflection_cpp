#pragma once

enum class StatesEnum {
  kAmbiguous,
  kName,
  kBrace,
  kConstant,
  kAliasing,
  kAliasingEnd,
  kEnd,
};