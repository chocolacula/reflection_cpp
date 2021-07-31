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

RR_OBJECT()
struct TheStruct2 {
  TheStruct2() {
  }

  std::string_view get_host() const {
    return _host;
  }

  int get_port() const {
    return _port;
  }

 private:
  RR_ALIAS(host)
  std::string _host = "localhost";

  RR_ALIAS(server_port)
  int _port = 8080;
};

RR_OBJECT()
struct TheStruct3 {

  TheStruct3() {
  }

  int id = 0;

  RR_OBJECT()
  struct TheStruct4 {
    std::string host = "localhost";

    RR_ALIAS(server_port)
    int port = 8080;
  } settings;
};
