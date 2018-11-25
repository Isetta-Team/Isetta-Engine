/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API SystemInfo {
 public:
  static std::string GetIPAddress();
  static std::string GetMachineName();
  static std::string GetSystemUserName();
};
}  // namespace Isetta
