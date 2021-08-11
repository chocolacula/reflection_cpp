#pragma once

namespace rr {

template <typename T>
struct is_string : std::false_type {};

template <typename T>
inline constexpr bool is_string_v = is_string<T>::value;

template <>
struct is_string<std::string> : std::true_type {};

template <>
struct is_string<std::string_view> : std::true_type {};

template <typename T>
struct is_sequence : std::false_type {};

template <typename T>
inline constexpr bool is_sequence_v = is_sequence<T>::value;

template <typename T>
struct is_sequence<std::vector<T>> : std::true_type {};

template <typename T>
struct is_sequence<std::list<T>> : std::true_type {};

template <typename T>
struct is_sequence<std::deque<T>> : std::true_type {};

template <typename T>
struct is_sequence<std::queue<T>> : std::true_type {};

template <typename T>
struct is_sequence<std::stack<T>> : std::true_type {};

template <typename T>
struct is_sequence<std::set<T>> : std::true_type {};

template <typename T>
struct is_sequence<std::unordered_set<T>> : std::true_type {};

template <typename T>
struct is_map : std::false_type {};

template <typename T>
inline constexpr bool is_map_v = is_map<T>::value;

template <typename KeyT, typename ValueT>
struct is_map<std::map<KeyT, ValueT>> : std::true_type {};

template <typename KeyT, typename ValueT>
struct is_map<std::unordered_map<KeyT, ValueT>> : std::true_type {};

template <typename T>
struct is_array {
  static constexpr bool value = std::is_array_v<T>;
};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

template <typename T, size_t size>
struct is_array<std::array<T, size>> : std::true_type {};

template <typename T>
struct is_class {
  static constexpr bool value = std::is_class_v<T> && !is_sequence_v<T> && !is_map_v<T> && !is_string_v<T>;
};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

}  // namespace rr
