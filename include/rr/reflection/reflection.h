#pragma once

#include <cstddef>
#include <iostream>

#include "printer.h"
#include "the_great_table.h"

namespace rr::Reflection {

TypeInfo reflect(Var variable) {
  return TheGreatTable::data()[variable.type().number()].reflect(const_cast<void*>(variable.raw()),
                                                                 variable.is_const());
}

template <typename T>
TypeInfo reflect(T* pointer) {
  return reflect(Var(pointer));
}

template <typename T>
TypeInfo reflect(const T* pointer) {
  return reflect(Var(pointer));
}

std::string_view type_name(TypeId id) {
  return TheGreatTable::data()[id.number()].type_name();
}

size_t type_size(TypeId id) {
  return TheGreatTable::data()[id.number()].type_size();
}

void* call_new(TypeId id, void* place, size_t place_size) {
  return TheGreatTable::data()[id.number()].call_new(place, place_size);
}

void call_delete(Var variable, bool in_place) {
  TheGreatTable::data()[variable.type().number()].call_delete(variable.raw_mut(), in_place);
}

Expected<None> copy(Var to, Var from) {
  if (to.is_const()) {
    return Error("Cannot assign to const value");
  }
  if (to.type() != from.type()) {
    return Error(format("Cannot copy {} to {}", type_name(from.type()), type_name(to.type())));
  }
  TheGreatTable::data()[to.type().number()].copy(to.raw_mut(), from.raw());
  return None();
}

}  // namespace rr::Reflection