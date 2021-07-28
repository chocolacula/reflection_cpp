#pragma once

#include "../../../generator/inja/third_party/include/nlohmann/json.hpp"
#include "../info/field_info.h"
#include "serializer.h"

namespace rr {

class SerializerNlohmannJson : public Serializer {

 public:
  SerializerNlohmannJson(nlohmann::json* json) : _json(json) {
  }

  void to_field(const char* name, FieldInfo info, void* object) override {

    (*kFunctions[info.id()])(&(*_json)[name], info, object);

  }

  void from_field(const char* name, FieldInfo info, const void* object) override {

    info.get(object).unbind([this, name](auto&& value) { (*_json)[name] = value; });
  }

 private:
  nlohmann::json* _json;

  static void write_float(nlohmann::json* json, FieldInfo info, void* object) {

    info.set(object, json->get<float>());
  }

  static void write_double(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<double>());
  }

  static void write_int8_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<int8_t>());
  }

  static void write_uint8_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<uint8_t>());
  }

  static void write_int16_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<int16_t>());
  }

  static void write_uint16_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<uint16_t>());
  }

  static void write_int32_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<int32_t>());
  }

  static void write_uint32_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<uint32_t>());
  }

  static void write_int64_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<int64_t>());
  }

  static void write_uint64_t(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<uint64_t>());
  }

  static void write_string(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<std::string>());
  }

  static void write_cstring(nlohmann::json* json, FieldInfo info, void* object) {

    info.set(object, json->get<std::string_view>().data());
  }

  static void write_string_view(nlohmann::json* json, FieldInfo info, void* object) {
    info.set(object, json->get<std::string_view>());
  }

  static constexpr inline void (*kFunctions[13])(nlohmann::json* json, FieldInfo info, void* object) = {
      write_float,        //
      write_double,       //
      write_int8_t,       //
      write_uint8_t,      //
      write_int16_t,      //
      write_uint16_t,     //
      write_int32_t,      //
      write_uint32_t,     //
      write_int64_t,      //
      write_uint64_t,     //
      write_string,       //
      write_cstring,      //
      write_string_view,  //
  };
};

}  // namespace rr