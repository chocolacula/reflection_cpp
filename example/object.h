#pragma once

#include <cstddef>

#include "rex/attributes.h"
#include "rex/info/type_info.h"

RR_OBJECT()
struct TheStruct1 {
  TheStruct1() {
  }

  std::string host = "localhost";

  RR_NAME(server_port)
  int port = 8080;

  RR_EXCLUDE
  int timeout = 120;
};

RR_OBJECT()
struct TheStruct2 {
  friend class rr::TypeInfo<TheStruct2>;

  TheStruct2() {
  }

  [[nodiscard]] std::string_view get_host() const {
    return _host;
  }

  [[nodiscard]] int get_port() const {
    return _port;
  }

 private:
  RR_NAME(host)
  std::string _host = "localhost";
  RR_NAME(server_port)
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

    RR_NAME(server_port)
    int port = 8080;
  } settings;
};
