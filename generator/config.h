#pragma once

#include <string>
#include <vector>

#include "rr/attributes.h"

using namespace std;

namespace olo1::ata1 {

enum class RR_REFLECT("") RR_REFLECT("")  //
    Colors{kWhite RR_ALIAS("white") RR_ALIAS("_white"), kRed, kGreen, kBlue, kBlack, kGray};

}  // namespace olo1::ata1

class ololo {};

struct Config;

struct RR_REFLECT("") Config {
  std::string root_dir;
  std::string compdb_dir;

  struct Templates {
    RR_ALIAS("enum")
    std::string for_enum = "blo";
    std::string object;
  } templates;

  static inline int VAR = 0;
  std::vector<std::string> input;
  std::string output_dir;
};