#pragma once

#include <optional>

class ICursorItem {
 public:
  std::optional<ICursorItem> next() = 0;
};
