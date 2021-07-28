#pragma once

#include "cursor_item.h"

template<typename PathT>
class ICursor : public ICursorItem {
 public:
  ICursor move(PathT path) = 0;
};
