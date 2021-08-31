#include <type_traits>

#include "../../expected.h"

template <typename T>
struct is_expected : std::false_type {};

template <typename T>
inline constexpr bool is_expected_v = is_expected<T>::value;  // NOLINT std like name

template <typename T, typename ErrorT>
struct is_expected<rr::Expected<T, ErrorT>> : std::true_type {};

/// Note: don't forget #undef it
#define __retry(x)                                                                           \
  {                                                                                          \
    auto __ex___ = x;                                                                        \
    static_assert(is_expected_v<decltype(__ex___)>,                                          \
                  "__retry(x) macro is compatible only with Expected<T, ErrorT> structure"); \
    if (__ex___.is_error()) {                                                                \
      return __ex___.error();                                                                \
    }                                                                                        \
  }
//
