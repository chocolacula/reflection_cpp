#pragma once

#include "rr/rex.h"

// clang-format off
REX_OBJECT
REX_WITH_PRIVATE
const struct TheStruct1 {

    TheStruct1(int int_argument) : _number(int_argument) {
    }

    REX_NAME(server_port)
    int port = 0;

    int get_number() {
      return _number;
    }

    REX_EXCLUDE
    int timeout = 0;

  private:
    REX_NAME(number)
    int _number;
} struct_variable(5);