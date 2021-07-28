#pragma once

#include <vector>

#include "rex/reflection/type_actions.h"
#include "strategies/array.h"
#include "strategies/vector.h"

namespace rr {

struct Sequence : public ISequence {

  Sequence() = delete;

  template <typename T, size_t size_v>
  explicit Sequence(T (*array)[size_v])
      : _sequence(std::make_shared<ArrayStrategy<T, size_v>>(array)), _nested_type(TypeId::get<T>()) {
  }

  template <typename T, size_t size_v>
  explicit Sequence(std::array<T, size_v>* array)
      : _sequence(std::make_shared<ArrayStrategy<T, size_v>>(array)), _nested_type(TypeId::get<T>()) {
  }

  template <template <typename T1> typename ContainerT, typename T>
  explicit Sequence(ContainerT<T>* container)  //
      : _sequence(choose(container)), _nested_type(TypeId::get<T>()) {
  }

  /* Expected<None, Error> remove(Var value) {
   * if (_type != value.type) {
   *   return Error("The Sequence type("+ _type.name() +") and value type(" +  value.type.name() + ") are different");
   * }
   *
   * SequenceHelper::remove(_type, _pointer, var.value);
  }*/

  Var var() override {
    return _sequence->var();
  }

  Expected<Var> first() override {
    return _sequence->first();
  };

  Expected<Var> last() override {
    return _sequence->last();
  };

  void for_each(std::function<void(Var)> callback) override {
    _sequence->for_each(callback);
  }

  void clear() override {
    _sequence->clear();
  }

  size_t size() override {
    return _sequence->size();
  }

  Expected<None> push(Var element) override {
    if (_nested_type != element.type()) {
      return Error(fmt::format("Trying to push value({}) to sequence<{}>",  //
                               TypeActions::type_name(element.type()),      //
                               TypeActions::type_name(_nested_type)));
    }

    return _sequence->push(element);
  }

 private:
  std::shared_ptr<ISequence> _sequence;
  TypeId _nested_type;

  template <typename T>
  std::shared_ptr<ISequence> choose(std::vector<T>* vector) {
    return std::make_shared<VectorStrategy<T>>(vector);
  }
};

}  // namespace rr