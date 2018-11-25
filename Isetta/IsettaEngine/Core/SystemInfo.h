/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "ISETTA_API.h"
#include "DataStructures/Array.h"

namespace Isetta {
class ISETTA_API SystemInfo {
 public:
  static Array<std::string> GetIPAddresses();
  static std::string GetMachineName();
  static std::string GetSystemUserName();
};
}  // namespace Isetta
