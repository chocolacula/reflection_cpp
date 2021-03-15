#pragma once

#include <cstring>
#include <string>
#include <string_view>

namespace rr {

union TightString {

  TightString(const TightString& data) {
    if (data.is_owned()) {
      new (&owned) std::string(data.owned);
    } else {
      zero_tail();
      shared = data.shared;
    }
  }

  TightString& operator=(const TightString& data) {
    if (data.is_owned()) {
      new (&owned) std::string(data.owned);
    } else {
      zero_tail();
      shared = data.shared;
    }

    return *this;
  }

  TightString(TightString&& data) {
    if (data.is_owned()) {
      new (&owned) std::string(std::move(data.owned));
    } else {
      zero_tail();
      shared = data.shared;
    }
  }

  TightString& operator=(TightString&& data) {
    if (data.is_owned()) {
      new (&owned) std::string(std::move(data.owned));
    } else {
      zero_tail();
      shared = data.shared;
    }

    return *this;
  }

  TightString(const char* shared) {
    zero_tail();
    this->shared = shared;
  }

  TightString(std::string_view shared) {
    zero_tail();
    this->shared = shared;
  }

  TightString(std::string&& owned) {
    new (&this->owned) std::string(std::move(owned));
  }

  TightString(){};

  ~TightString() {
    if (is_owned())
      owned.~basic_string();
  };

  [[nodiscard]] bool is_owned() const {
    auto const n = sizeof(std::string) / sizeof(unsigned long);
    return (_raw[n - 2] | _raw[n - 1]) != 0;
  }

  std::string_view shared;
  std::string owned;

 private:
  inline void zero_tail() {
    std::memset(&_raw[sizeof(std::string_view) / sizeof(unsigned long)], 0,
                sizeof(std::string) - sizeof(std::string_view));
  }

  unsigned long _raw[sizeof(std::string) / sizeof(unsigned long)];
};

}  // namespace rr