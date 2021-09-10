#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "../registry/node.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "nlohmann/json.hpp"
#include "rr/info/access.h"

using namespace rr;

class ParserCpp {
 public:
  explicit ParserCpp() {
  }

  void get_node(std::string_view file_path) {
  }

 private:
  struct Entry {
    Entry() : offset(0), name(), level(0) {
    }
    Entry(int offset, std::string name, size_t level) : offset(offset), name(std::move(name)), level(level) {
    }
    int offset;
    std::string name;
    size_t level;
  };

  struct Data {
    Data() : macro(), tokens(nullptr), parse_through(false), current_level(0) {
    }
    explicit Data(std::vector<Entry>* tokens) : macro(), tokens(tokens), parse_through(false), current_level(0) {
    }
    std::queue<Entry> macro;
    std::vector<Entry>* tokens;
    bool parse_through;
    size_t current_level;
  };

  std::vector<Entry> _tokens{};
};