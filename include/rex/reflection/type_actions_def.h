#pragma once

#include "../the_great_table.h"

namespace rr {

std::string_view TypeActions::type_name(TypeId id) {
  return TheGreatTable::data()[id.number()].type_name();
}

size_t TypeActions::type_size(TypeId id) {
  return TheGreatTable::data()[id.number()].type_size();
}

}  // namespace rr