#pragma once

namespace rr {

struct UnknownTypeHelper {
  static TypeInfo reflect(void* value) {
    throw std::runtime_error("Cannot reflect a value with unknown type");
  }

  static std::string_view type_name() {
    return "unknown";
  }

  static size_t type_size() {
    return 0;
  }

  static Var alloc_default() {
    throw std::runtime_error("Cannot alloc a value for unknown type");
  }

  static void call_delete(void* pointer) {
    throw std::runtime_error("Cannot delete a value for unknown type");
  }

  static void copy(void* from, void* to) {
    throw std::runtime_error("Cannot copy values for unknown type");
  }

  static bool copy_default(void* to, size_t size) {
    throw std::runtime_error("Cannot copy a value for unknown type");
  }

  // static Var dictionary_alloc_value(void* key_pointer, Dictioanary* dictioanary) {
  //
  //   return dictioanary->push_default(static_cast<float*>(key_pointer));
  // }

  // other types are self registering in The Great Table in the same place of a file
  // but HelperUnknown is NOT a self registering class
  // The Great Table put references to HelperUnknown methods itself
};

}  // namespace rr