#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace rr {

template <int N>
class CInt : public CInt<N - 1> {
 public:
  static constexpr const int kValue = N;
};

template <>
class CInt<0> {
 public:
  static constexpr const int kValue = 0;
};

struct TypeId {

  template <typename T>
  static constexpr int get() {
    return -1;
  }

  static constexpr int max_id() {
    return kId;
  }

 private:
  static constexpr inline int kId = 13;
};

template <>
constexpr int TypeId::get<float>() {
  return 0;
}

template <>
constexpr int TypeId::get<double>() {
  return 1;
}

template <>
constexpr int TypeId::get<int8_t>() {
  return 2;
}

template <>
constexpr int TypeId::get<uint8_t>() {
  return 3;
}

template <>
constexpr int TypeId::get<int16_t>() {
  return 4;
}

template <>
constexpr int TypeId::get<uint16_t>() {
  return 5;
}

template <>
constexpr int TypeId::get<int32_t>() {
  return 6;
}

template <>
constexpr int TypeId::get<uint32_t>() {
  return 7;
}

template <>
constexpr int TypeId::get<int64_t>() {
  return 8;
}

template <>
constexpr int TypeId::get<uint64_t>() {
  return 9;
}

template <>
constexpr int TypeId::get<std::string>() {
  return 10;
}

template <>
constexpr int TypeId::get<const char*>() {
  return 11;
}

template <>
constexpr int TypeId::get<std::string_view>() {
  return 12;
}

}  // namespace rr