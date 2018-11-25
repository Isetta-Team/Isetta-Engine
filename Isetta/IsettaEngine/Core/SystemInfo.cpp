/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/SystemInfo.h"
#include <windows.networking.sockets.h>
#include "Config/Config.h"

namespace Isetta {

std::string SystemInfo::GetSubnetIPAddress() {
  for (auto ipAddress : GetIPAddresses()) {
    if (ipAddress.find(CONFIG_VAL(networkConfig.ipPrefix)) == 0) {
      return ipAddress;
    }
  }
  LOG_ERROR(Debug::Channel::Networking,
            "Did find a good subnet ip that starts with %s",
            CONFIG_VAL(networkConfig.ipPrefix));
  return std::string{};
}

// Adapted from: https://tangentsoft.net/wskfaq/examples/ipaddr.html
Array<std::string> SystemInfo::GetIPAddresses() {
  Array<std::string> ipAddresses;

  WSAData wsaData{};
  if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
    LOG_ERROR(Debug::Channel::Networking, "Cannot get IP info.");
    return ipAddresses;
  }

  struct hostent* hostInfo = gethostbyname(GetMachineName().c_str());
  if (hostInfo == nullptr) {
    LOG_ERROR(Debug::Channel::Networking, "Cannot get IP info.");
    return ipAddresses;
  }

  for (int i = 0; hostInfo->h_addr_list[i] != nullptr; ++i) {
    struct in_addr address {};
    memcpy(&address, hostInfo->h_addr_list[i], sizeof(struct in_addr));
    ipAddresses.PushBack(std::string{inet_ntoa(address)});
  }

  WSACleanup();
  return ipAddresses;
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
