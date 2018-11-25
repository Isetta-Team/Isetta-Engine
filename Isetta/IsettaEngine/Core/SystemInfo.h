/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "DataStructures/Array.h"
#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API SystemInfo {
 public:
  static std::string GetIpAddressWithPrefix(std::string_view prefix);
  static Array<std::string> GetIPAddresses();
  static std::string GetMachineName();
  static std::string GetSystemUserName();
};
}  // namespace Isetta
