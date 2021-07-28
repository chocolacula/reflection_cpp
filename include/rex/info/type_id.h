#pragma once

#include <array>
#include <string_view>
#include <vector>

namespace rr {

struct TypeId {

  TypeId() = delete;

  /// the main function of TypeId mechanism
  /// it is specializing for each type in some files
  template <typename T>
  static TypeId get(T* ptr) {
    return TypeId(0);
  }

  /// proxy function for using with implicit type in TypeId::get<T>() way without argument
  template <typename T>
  static TypeId get() {
    return get(static_cast<T*>(nullptr));
  }

  // template <typename T>
  // static TypeId get(const T& ref) {
  //   return TypeId(0);
  // }

  // only definitions, it works throughout overloading, without specialization
  template <typename T>
  static TypeId get(std::vector<T>* vector);

  template <typename T, size_t size>
  static TypeId get(T (*array)[size]);

  template <typename T, size_t size>
  static TypeId get(std::array<T, size>* array);

  bool operator==(const TypeId& other) const {
    return _id == other._id;
  }

  bool operator!=(const TypeId& other) const {
    return _id != other._id;
  }

  size_t number() const {
    return _id;
  }

 private:
  size_t _id;

  explicit TypeId(size_t id) : _id(id) {
  }
};

// template <typename T>
// constexpr TypeId TypeId::get(T[] /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T, size_t N>
// constexpr TypeId TypeId::get(std::array<T, N>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T>
// constexpr TypeId TypeId::get(std::vector<T>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T>
// constexpr TypeId TypeId::get(std::list<T>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T>
// constexpr TypeId TypeId::get(std::stack<T>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T>
// constexpr TypeId TypeId::get(std::deque<T>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T>
// constexpr TypeId TypeId::get(std::unordered_set<T>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename T>
// constexpr TypeId TypeId::get(std::set<T>& /**/) {
//   auto id = TypeId::get<T>();
//   id._is_sequence = true;
//
//   return id;
// }
//
// template <typename KeyT, typename ValueT>
// constexpr TypeId TypeId::get(std::unordered_map<KeyT, ValueT>& /**/) {
//   TypeId id(TypeId::get<KeyT>(), TypeId::get<ValueT>());
//   id._is_dictionary = true;
//
//   return id;
// }
//
// template <typename KeyT, typename ValueT>
// constexpr TypeId TypeId::get(std::map<KeyT, ValueT>& /**/) {
//   TypeId id(TypeId::get<KeyT>(), TypeId::get<ValueT>());
//   id._is_dictionary = true;
//
//   return id;
// }

}  // namespace rr