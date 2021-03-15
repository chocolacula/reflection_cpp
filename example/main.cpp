
// #include "example/struct.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <typeinfo>

#include "enum.h"
#include "nlohmann/json.hpp"
#include "object.h"
#include "reflection.h"
#include "rex/info/type_id.h"
#include "rex/info/type_info.h"
#include "rex/serializer/nlohmann_json_adapter.h"

using namespace rr;

int main() {

  // You can get an Enum constant string representation
  auto expected_string = EnumInfo<TheEnumClass>::to_string(TheEnumClass::kWhite);

  // and check if it does exist and everything got right
  if (!expected_string.is_error()) {
    std::cout << "it's a string representation of " << expected_string.get() << "!" << std::endl;
  }

  // or use type matching
  expected_string.unbind([](std::string_view str) { std::cout << "Unbind " << str << std::endl; },
                         [](Error err) { std::cout << err.what() << std::endl; });

  // if the constant doesn't exist
  auto expected_enum = EnumInfo<TheEnumClass>::parse("kGray");

  // An Error object with message will be returned
  expected_enum.unbind(  //
      [](TheEnumClass /*constant*/) { std::cout << "Unbind a constant" << std::endl; },
      [](Error err) { std::cout << "\nCopy Error object and print message:\n"
                                << err.what() << std::endl; });

  // Move version also available
  expected_enum.unbind_move(  //
      [](TheEnumClass /*constant*/) { std::cout << "Unbind a constant" << std::endl; },
      [](Error err) { std::cout << "\nMove Error object and print message only once\n"
                                << err.what() << std::endl; });

  std::cout << std::endl;

  // It works for old plain enum type but in this case there is no any generated code for TheEnum
  EnumInfo<TheEnum>::to_string(TheEnum::kBlack)
      .unbind([](std::string_view str) { std::cout << str << std::endl; },
              [](Error err) { std::cout << err.what() << std::endl; });

  std::cout << std::endl;

  // And ofcourse you can use structs and classes via TypeInfo if they are standard_layout data types
  TheStruct1 struct1;

  // TypeInfo may be used in runtime by TypeId and global function
  // or in compile time template class like TypeInfo<TheStruct1>::get_field
  auto expected_value = type_info(TypeId::get<TheStruct1>()).get_field("server_port").get().get(&struct1);

  // ExpectedValue doesn't contain an Error because FieldInfo guarantees that it doesn't contain invalid value
  // but nobody knows which type is it, to figure out it use type matching
  expected_value.unbind([](int v) { std::cout << "This is int! " << v << std::endl; },
                        [](auto&& /*v*/) { std::cout << "This is not int!" << std::endl; });

  // ExpectedValue will return string_view even the field has string type
  expected_value = TypeInfo<TheStruct1>::get_field("host").get().get(&struct1);

  expected_value.unbind([](std::string_view v) { std::cout << "This is string_view! \"" << v << "\"" << std::endl; },
                        [](auto&& /*v*/) { std::cout << "This is not string!" << std::endl; });

  std::cout << std::endl;

  // Setting a value also possible with runtime but fast type checking
  auto ok = TypeInfo<TheStruct1>::get_field("server_port").get().set(&struct1, 9000);

  if (ok.is_error()) {
    std::cout << "An int value didn't set " << std::endl;
  } else {
    std::cout << "Set new int value " << std::endl;
  }

  ok = TypeInfo<TheStruct1>::get_field("server_port").get().set(&struct1, 9000.2);

  if (ok.is_error()) {
    std::cout << "A double value didn't set " << std::endl;
  } else {
    std::cout << "Set new double value " << std::endl;
  }

  std::cout << "\nThe field server_port now is " << struct1.port << "\n" << std::endl;

  // And for sweet one you can serialize to struct and vice vera
  auto input = nlohmann::json::parse(R"({ "host": "google.com", "server_port": 5000, "timeout": 10 })");
  SerializerNlohmannJson input_adapter(&input);

  TheStruct1 struct_new;

  TypeInfo<TheStruct1>::deserialize(&struct_new, &input_adapter);

  struct_new.port = 5080;

  nlohmann::json output;
  SerializerNlohmannJson output_adapter(&output);
  TypeInfo<TheStruct1>::serialize(&struct_new, &output_adapter);

  std::cout << "struct_new after serialization is:\n" << output << std::endl;

  // Deserialization of structs with private fields possible too
  // but it must contains ALL private fields to be standard_layout data type
  TheStruct2 struct2;
  TypeInfo<TheStruct2>::deserialize(&struct2, &input_adapter);

  std::cout << "\nstruct2 has only private fields:"        //
            << "\n_host=\"" << struct2.get_host() << "\""  //
            << "\n_port=" << struct2.get_port() << std::endl;
}