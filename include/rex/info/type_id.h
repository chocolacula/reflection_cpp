#pragma once

#include <array>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../traits.h"

namespace rr {

struct TypeId {
  TypeId() = delete;

  /// proxy function for using with implicit type in TypeId::get<T>() way without argument
  template <typename T>
  static TypeId get() {
    return get(static_cast<T*>(nullptr));
  }

  /// the main function of TypeId mechanism
  template <typename T>
  static typename std::enable_if_t<is_class_v<T>, TypeId>  //
  get(T* ptr) {
    return TypeId(0);
  }

  template <typename T>
  static typename std::enable_if_t<std::is_enum_v<T>, TypeId>  //
  get(T* ptr) {
    return TypeId(0);
  }

  template <typename T>
  static typename std::enable_if_t<std::is_integral_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T>
  static typename std::enable_if_t<std::is_floating_point_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T>
  static typename std::enable_if_t<is_string_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T, size_t size>
  static TypeId get(T (*array)[size]);

  template <typename T, size_t size>
  static TypeId get(std::array<T, size>* array);

  template <typename T>
  static typename std::enable_if_t<is_sequence_v<T>, TypeId>  //
  get(T* ptr);

  template <typename T>
  static typename std::enable_if_t<is_map_v<T>, TypeId>  //
  get(T* ptr);

  bool operator==(const TypeId& other) const {
    return _id == other._id;
  }

  bool operator!=(const TypeId& other) const {
    return _id != other._id;
  }

  uint32_t number() const {
    return _id;
  }

 private:
  uint32_t _id;

  explicit TypeId(uint32_t id) : _id(id) {
  }
};

}  // namespace rr