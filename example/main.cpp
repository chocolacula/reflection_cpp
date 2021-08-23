#include <cfloat>
#include <string_view>
#include <utility>

#include "handwritten/reflection.h"
#include "print.h"
#include "rr/reflection/printer.h"
#include "rr/serializer/json.h"

using namespace rr;

int main() {
  // You can get an Enum constant string representation by reflecting them
  auto white = Colors::kWhite;

  // Reflection::reflect(ptr) returns a TypeInfo structure
  // which could represent Integer, Float, Enum and other kinds of types
  auto white_info = Reflection::reflect(&white);

  // You can check kind of the TypeInfo by calling is<Type>() and get it further
  if (white_info.is<Enum>()) {
    println("it's a string representation of {}", white_info.get<Enum>().to_string());
  }

  // or use type matching
  white_info.match([](Enum& e) { println("Matched and printed: {}", e.to_string()); },
                   [](auto&& /*unused*/) { println("It's not an enum"); });

  println();

  // it is possible to parse enum constant from string value too
  auto the_enum = white_info.get<Enum>();

  the_enum.parse("kPurple").match(  //
      [&](None) { println("kWhite recolored to {}", the_enum.to_string()); },
      [](Error& err) { println("Got an error message: {}", err.what()); });

  // if the constant doesn't exist
  // an Error object with message will be returned
  the_enum.parse("kPurple").match(  //
      [](None) { println("Got an enum constant"); },
      [](Error& err) { println("Got an error message: {}", err.what()); });

  println();

  // it works for old plain enum type but in this case there isn't any generated code for TheEnum
  auto giraffe = Animals::kGiraffe;
  try {
    auto giraffe_info = Reflection::reflect(&giraffe);
  } catch (const std::exception& ex) {
    println("Reflection throws the exception: '{}'", ex.what());
  }

  println();

  // and of course you can use structs and classes
  Bicycle gt_avalanche;
  gt_avalanche.id = 34894613;
  gt_avalanche.manufacturer = "GT";
  gt_avalanche.model = "Avalanche";
  gt_avalanche.wheel_size_inch = 26;
  gt_avalanche.frame_weight = 12;
  gt_avalanche.colors = {Colors::kBlack, Colors::kRed};

  auto bicycle_info = Reflection::reflect(&gt_avalanche);

  // setting a value is possible with runtime but fast type checking and casting
  // Note: field 'frame_weight' reflected with alias 'weight'
  auto* field_p = bicycle_info.get<Object>().get_field("weight").unwrap().rt_cast<float>().unwrap();
  *field_p = 14.9F;

  // or by using separate reflection step for a field
  auto field_var = bicycle_info.get<Object>().get_field("weight").unwrap();
  auto ok = Reflection::reflect(field_var).get<Float>().set(DBL_MAX);

  if (ok.is_error()) {
    println("The value too big for a float field");
  } else {
    println("New value has been set");
  }

  // for debug purposes there is a possibility to print out struct content
  // via print() and sprint() functions in Reflection namespace
  println("\n{}", Reflection::sprint(bicycle_info));

  // and for a sweet one you can serialize to struct and vice vera
  // auto input = nlohmann::json::parse(R"({ "host": "google.com", "server_port": 5000, "timeout": 10 })");
  // SerializerNlohmannJson input_adapter(&input);

  Bicycle norco_aurum;
  norco_aurum.id = 17893448;
  norco_aurum.is_hardtail = false;
  norco_aurum.manufacturer = "Norco";
  norco_aurum.model = "Aurum";
  norco_aurum.wheel_size_inch = 27.5;
  norco_aurum.frame_weight = 16.4;
  norco_aurum.colors = {Colors::kGray, Colors::kGreen};

  Reflection::print(&norco_aurum);
  println();

  std::cout << json::to_string(&gt_avalanche).unwrap() << std::endl;

  return 0;
}