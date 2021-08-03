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
      [](Colors constant) { fmt::print("kGreen has integer value: {}\n", static_cast<int>(constant)); },
      [](Error err) { fmt::print("Got an error: {}\n", err.what()); });

  std::cout << "\n";

  // if the constant doesn't exist
  expected_enum = EnumInfo<Colors>::parse("kGray");

  // An Error object with message will be returned
  expected_enum.match(  //
      [](Colors /*constant*/) { fmt::print("Got an enum constant\n"); },
      [](Error err) { fmt::print("Got an error message: {}\n", err.what()); });

  std::cout << "\n";

  // It works for old plain enum type but in this case there is no any generated code for TheEnum
  EnumInfo<Animals>::to_string(Animals::kGiraffe)
      .match([](std::string_view str) { fmt::print("{}\n", str); },
             [](Error err) { fmt::print("Cannot convert an enum to string, error: {}\n", err.what()); });

  std::cout << "\n";

  //
  //
  //
  TheStruct1 s;
  auto s1_info = Reflection::reflect(&s);

  std::cout << Reflection::type_name(TypeId::get(&s.error_codes)) << "\n";
  Reflection::print(s1_info);

  auto field_info = Reflection::reflect(&s.error_codes);

  field_info.match(
      [](Sequence& s) {
        auto is_v = s.is<Vector>();
        auto& v = s.get<Vector>();

        Reflection::reflect(v[1].unwrap())
            .match(
                [](Sequence& s) {
                  s.clear();

                  // {
                  //   int v = 1;
                  //   s.push(Var(&v));
                  // }
                  {
                    int v = 2;
                    s.push(Var(&v));
                  }
                  // {
                  //   int v = 3;
                  //   s.push(Var(&v));
                  // }
                },
                [](auto&&) {});
      },
      [](auto&&) {});

  Reflection::print(s1_info);

  int arr[6] = {11, 12, 13};
  Reflection::print(&arr);
  std::cout << Reflection::type_name(TypeId::get(&arr)) << std::endl;

  std::array<int, 4> std_arr = {7, 8};
  Reflection::print(Reflection::reflect(&std_arr));
  std::cout << Reflection::type_name(TypeId::get(&std_arr)) << std::endl;

  std::unordered_map<int, std::string_view> m = {{1, "1"}, {33, "thirty three"}};
  std::cout << Reflection::type_name(TypeId::get(&m)) << std::endl;
  Reflection::print(&m);

  TheStruct2 s2;
  Reflection::print(&s2);

  return 0;
}