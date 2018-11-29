/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>

namespace Isetta {
struct ClientInfo {
  std::string ip{};
  std::string machineName{};
  int clientIndex{-1};
  ClientInfo() = default;
  ClientInfo(const std::string& ip, const std::string& machineName,
             const int clientIndex)
      : ip(ip), machineName(machineName), clientIndex(clientIndex) {}
};
}  // namespace Isetta
