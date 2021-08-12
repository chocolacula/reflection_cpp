#pragma once

#include <streambuf>
// #include <iostream>
// #include <ostream>
// #include <string>

namespace rr {

class AppendBuf : public std::streambuf {
 public:
  explicit AppendBuf(std::string* str) : _buf(str) {
  }

  virtual int_type overflow(int_type c) {
    if (c != EOF) {
      _buf->push_back(c);
    }
    return c;
  }

  virtual std::streamsize xsputn(const char* s, std::streamsize n) {
    _buf->insert(_buf->end(), s, s + n);
    return n;
  }

 private:
  std::string* _buf;
};

}  // namespace rr
