#pragma once

#include "rex/info/type_id.h"

struct Dictionary {

  template<template<typename T1, typename T2> typename ContainerT, typename KeyT, typename ValueT>
  Dictionary(ContainerT<KeyT, SecondT>* container)
      : _pointer(container),
        _key_type(TypeId<KeyT>()),
        _value_type(TypeId<ValueT>()) {
  }

  /// returns hidden value with type checking
  // Expexted<T&, Error> get<T>();

  // Expected<None, Error> insert(Var key, Var value)

  // Expected<None, Error> remove(Var key)

  // void clear()

  // size_t size()

  // begin()
  // end()

 private:
  void* _pointer; // the single one pointer for the container
  TypeId _key_type;
  TypeId _value_type;
};