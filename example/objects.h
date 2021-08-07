#pragma once

#include <cstddef>

#include "rex/attributes.h"

RR_OBJECT()
struct TheStruct1 {
  TheStruct1() {
  }

  std::string host = "localhost";

  RR_ALIAS(server_port)
  int port = 8080;

  std::vector<std::vector<int>> error_codes = {{400, 404, 502}, {600, 601}};

  RR_EXCLUDE
  float timeout = 6.5;
};
