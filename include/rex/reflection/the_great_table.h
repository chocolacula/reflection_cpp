#pragma once

#include "../types/unknown.h"
#include "actions.h"

namespace rr {

/// the main component of reflection system - big table with function pointers
/// each reflectable type has to have a record in the table
struct TheGreatTable {

  static const std::vector<Actions>& data() {
    return _data;
  }

  static size_t record(Actions actions) {
    _data.push_back(actions);
    return _data.size() - 1;
  }

 private:
  // zero index for unknown type
  static inline std::vector<Actions> _data = {Actions(&UnknownTypeHelper::reflect,        //
                                                      &UnknownTypeHelper::type_name,      //
                                                      &UnknownTypeHelper::type_size,      //
                                                      &UnknownTypeHelper::alloc_default,  //
                                                      &UnknownTypeHelper::call_delete,    //
                                                      &UnknownTypeHelper::copy,           //
                                                      &UnknownTypeHelper::copy_default)};
};

}  // namespace rr