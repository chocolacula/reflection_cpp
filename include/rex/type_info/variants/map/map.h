#pragma once

#include "imap.h"
#include "std_map.h"
#include "std_unordered_map.h"

namespace rr {

struct Map final : public IMap {

  template <typename KeyT, typename ValueT>
  Map(std::map<KeyT, ValueT>* map) : _map(std::make_shared<StdMap<KeyT, ValueT>>(map)) {
  }

  template <typename KeyT, typename ValueT>
  Map(std::unordered_map<KeyT, ValueT>* map) : _map(std::make_shared<StdUnorderedMap<KeyT, ValueT>>(map)) {
  }

  Var own_var() override {
    return _map->own_var();
  }

  void for_each(std::function<void(Var, Var)> callback) override {
    return _map->for_each(callback);
  }

  void clear() override {
  }

  size_t size() override {
    return _map->size();
  }

  Expected<None> insert(Var key, Var value) override {
    return _map->insert(key, value);
  }

  Expected<None> remove(Var key) override {
    return _map->remove(key);
  }

 private:
  std::shared_ptr<IMap> _map;
};

}  // namespace rr