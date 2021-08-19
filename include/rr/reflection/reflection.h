#pragma once

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

Var alloc_default(TypeId id) {
  return TheGreatTable::data()[id.number()].alloc_default();
}

void call_delete(Var variable) {
  TheGreatTable::data()[variable.type().number()].call_delete(variable.raw_mut());
}

void copy(Var to, Var from) {
  TheGreatTable::data()[from.type().number()].copy(to.raw_mut(), from.raw());
}

bool copy_default(TypeId id, void* to, size_t size) {
  return TheGreatTable::data()[id.number()].copy_default(to, size);
}

}  // namespace rr::Reflection