#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "rr/info/access.h"

namespace options {
const std::string_view kWithBase = "WithBase";
const std::string_view kWithPrivate = "WithPrivate";
}  // namespace options

enum class Kind { kEnum, kEnumClass, kStruct, kClass };

struct Item {
  Item(const std::string& name, const std::string& alias, rr::Access access)
      : name(name), alias(alias), access(access) {
  }

  std::string name;
  std::string alias;
  rr::Access access;
};

struct ParsedEntity {
  std::string name;

  std::string current_namespace;
  std::vector<std::string> used_namespaces;

  std::vector<std::string> parents;

  Kind kind;

  std::vector<Item> items;
};
