#pragma once

#include "cursor.h"

template<typename PathT>
class IWriter : public ICursor<PathT> {
 public:
  template<typename ValueT>
  void write(const ValueT& value) = 0;
};
