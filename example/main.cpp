#include <iostream>

#include "handwritten/reflection.h"
#include "objects.h"

using namespace rr;

int main() {

  // You can get an Enum constant string representation
  auto expected_string = EnumInfo<Colors>::to_string(Colors::kWhite);

  // and check if it does exist and everything got right
  if (!expected_string.is_error()) {
    fmt::print("it's a string representation of {}\n", expected_string.unwrap());
  }

  // or use type matching
  expected_string.match([](std::string_view str) { fmt::print("Matched and printed: {}\n", str); },
                        [](Error err) { fmt::print("{}\n", err.what()); });

  std::cout << "\n";

  // it is possible to parse enum constant from string value too
  auto expected_enum = EnumInfo<Colors>::parse("kGreen");
  expected_enum.match(  //
      [](Colors constant) { fmt::print("kGreen has int value: {}\n", static_cast<int>(constant)); },
      [](Error err) { fmt::print("Got an error: {}\n", err.what()); });

  std::cout << "\n";

  // if the constant doesn't exist
  expected_enum = EnumInfo<Colors>::parse("kGray");

  // An Error object with message will be returned
  expected_enum.match(  //
      [](Colors /*constant*/) { fmt::print("Got an enum constant\n"); },
      [](Error err) { fmt::print("Copy Error object and print message: {}\n", err.what()); });

  // Move version also available
  expected_enum.match_move(  //
      [](Colors /*constant*/) { fmt::print("Got a constant\n"); },
      [](Error err) { fmt::print("Move Error object and print message: {}\n", err.what()); });

  std::cout << "\n";

  // It works for old plain enum type but in this case there is no any generated code for TheEnum
  EnumInfo<Animals>::to_string(Animals::kGiraffe)
      .match([](std::string_view str) { fmt::print("{}\n", str); },
             [](Error err) { fmt::print("Cannot translate an enum to string, error: {}\n", err.what()); });

  std::cout << "\n";

  //
  //
  //
  TheStruct1 s;

  auto info = Reflection::reflect(&s);

  std::cout << Reflection::type_name(TypeId::get(&s.error_codes)) << "\n";
  Reflection::print(info);

  auto field_info = Reflection::reflect(&s.error_codes);

  field_info.match(
      [](Sequence s) {
        bool done = false;
        s.for_each([&done](Var entry) {
          auto sub_seq_info = Reflection::reflect(entry);

          if (!done) {
            sub_seq_info.match(
                [](Sequence s) {
                  s.clear();

                  {
                    int v = 1;
                    s.push(Var(&v));
                  }
                  {
                    int v = 2;
                    s.push(Var(&v));
                  }
                  {
                    int v = 3;
                    s.push(Var(&v));
                  }
                },
                [](auto&&) {});
          }

          done = true;
        });
      },
      [](auto&&) {});

  Reflection::print(info);

  int arr[] = {11, 12, 13};
  std::array<int, 2> std_arr = {7, 8};

  std::cout << Reflection::type_name(TypeId::get(&arr)) << std::endl;
  std::cout << Reflection::type_name(TypeId::get(&std_arr)) << std::endl;

  auto std_arr_info = Reflection::reflect(&arr);
  Reflection::print(std_arr_info);

  Reflection::print(Reflection::reflect(&std_arr));

  return 0;
}