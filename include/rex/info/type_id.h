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
  template <typename T, size_t size>
  static TypeId get(T (*array)[size]);

  template <typename T, size_t size>
  static TypeId get(std::array<T, size>* array);

  template <typename T>
  static TypeId get(std::vector<T>* vector);

  template <typename T>
  static TypeId get(std::list<T>* list);

  template <typename T>
  static TypeId get(std::stack<T>* stack);

  template <typename T>
  static TypeId get(std::queue<T>* queue);

  template <typename T>
  static TypeId get(std::deque<T>* deque);

  template <typename T>
  static TypeId get(std::set<T>* set);

  template <typename T>
  static TypeId get(std::unordered_set<T>* unordered_set);

  template <typename KeyT, typename ValueT>
  static TypeId get(std::map<KeyT, ValueT>* map);

  template <typename KeyT, typename ValueT>
  static TypeId get(std::unordered_map<KeyT, ValueT>* unordered_map);

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