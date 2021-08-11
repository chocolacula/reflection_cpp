#include <fmt/core.h>

#include <iostream>

#include "handwritten/reflection.h"
#include "rex/serializer/native.h"

using namespace rr;

int main() {
  // You can get an Enum constant string representation by reflecting them
  auto white = Colors::kWhite;
  auto white_info = Reflection::reflect(&white);

  // and check if it does exist and everything got right
  if (white_info.is<Enum>()) {
    fmt::print("it's a string representation of {}\n", white_info.get<Enum>().to_string());
  }

  // or use type matching
  white_info.match([](Enum& e) { fmt::print("Matched and printed: {}\n", e.to_string()); },
                   [](auto&&) { fmt::print("It's not an enum"); });

  std::cout << "\n";

  // it is possible to parse enum constant from string value too
  auto the_enum = white_info.get<Enum>();

  the_enum.parse("kGreen").match(  //
      [&](None) { fmt::print("kWhite recolored to {}\n", the_enum.to_string()); },
      [](Error& err) { fmt::print("Got an error message: {}\n", err.what()); });

  std::cout << "\n";

  // if the constant doesn't exist
  // an Error object with message will be returned
  the_enum.parse("kGray").match(  //
      [](None) { fmt::print("Got an enum constant\n"); },
      [](Error& err) { fmt::print("Got an error message: {}\n", err.what()); });

  std::cout << "\n";

  // It works for old plain enum type but in this case there isn't any generated code for TheEnum
  auto giraffe = Animals::kGiraffe;
  try {
    auto giraffe_info = Reflection::reflect(&giraffe);
  } catch (const std::exception& ex) {
    fmt::print("Reflection throws the exception: '{}'\n", ex.what());
  }

  std::cout << "\n";

  // And of course you can use structs and classes
  Bicycle bike;

  bike.id = 34894613;
  bike.manufacturer = "GT";
  bike.model = "Avalanche";
  bike.wheel_size_inch = 26;
  bike.frame_weight = 14.5;
  bike.colors = {Colors::kBlack, Colors::kRed};

  Reflection::print(&bike);

  /*
      // TypeInfo may be used in runtime by TypeId and global function
      // or in compile time template class like TypeInfo<TheStruct1>::get_field
      auto expected_value = type_info(TypeId::get<TheStruct1>()).get_field("server_port").unwrap().unwrap(&struct1);

      // ExpectedValue doesn't contain an Error because FieldInfo guarantees that it doesn't contain invalid value
      // but nobody knows which type is it, to figure out it use type matching
      expected_value.match([](int v) { std::cout << "This is int! " << v << std::endl; },
                           [](auto&& v) { std::cout << "This is not int!" << std::endl; });

      // ExpectedValue will return string_view even the field has string type
      expected_value = TypeInfo<TheStruct1>::get_field("host").unwrap().get(&struct1);

      expected_value.match([](std::string_view v) { std::cout << "This is string_view! \"" << v << "\"" <<
     std::endl; },
                           [](auto&& v) { std::cout << "This is not string!" << std::endl; });

      std::cout << std::endl;

      // Setting a value also possible with runtime but fast type checking
      auto ok = TypeInfo<TheStruct1>::get_field("server_port").unwrap().set(&struct1, 9000);

      if (ok.is_error()) {
        std::cout << "An int value didn't set " << std::endl;
      } else {
        std::cout << "Set new int value " << std::endl;
      }

      ok = TypeInfo<TheStruct1>::get_field("server_port").unwrap().set(&struct1, 9000.2);

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
    */
  return 0;
}