#pragma once

#include "cursor.h"

template<typename PathT>
class IReader : public ICursor<PathT> {
 public:
  template<typename ValueT>
  ValueT read();
};
