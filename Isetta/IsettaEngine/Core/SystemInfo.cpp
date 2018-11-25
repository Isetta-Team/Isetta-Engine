/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/SystemInfo.h"
#include "Config/Config.h"

namespace Isetta {

std::string SystemInfo::GetIPAddress() {
  return CONFIG_VAL(networkConfig.defaultClientIP);
}

std::string SystemInfo::GetMachineName() {
  char* buf = nullptr;
  Size size = 0;
  _dupenv_s(&buf, &size, "COMPUTERNAME");
  std::string ret = buf;
  free(buf);
  return ret;
}

std::string SystemInfo::GetSystemUserName() {
  char* buf = nullptr;
  Size size = 0;
  _dupenv_s(&buf, &size, "USERNAME");
  std::string ret = buf;
  free(buf);
  return ret;
}
}  // namespace Isetta
