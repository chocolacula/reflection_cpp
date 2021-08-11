#pragma once

namespace rr {

struct Bool {
  Bool(bool* value, bool is_const) : _value(value), _is_const(is_const) {
  }

  bool get() const {
    return *_value;
  }

  void set(bool value) {
    *_value = value;
  }

  std::string_view to_string() const {
    return *_value ? "true" : "false";
  }

  void parse(std::string_view str) {
    std::string low(str);
    std::transform(low.begin(), low.end(), low.begin(), ::tolower);
    *_value = str == "true";
  }

 private:
  bool* _value;
  const bool _is_const;
};

}  // namespace rr