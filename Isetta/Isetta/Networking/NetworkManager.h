/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Networking/NetworkingModule.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {
class NetworkManager {
 public:
  static void SendHandleMessageFromClient(int handle);
  static void SendStringMessageFromClient(std::string string);
  
  static void SendHandleMessageFromServer(int clientIdx, int handle);
  static void SendStringMessageFromServer(int clientIdx, std::string string);

  static void ConnectToServer(const char* serverAddress,
                        std::function<void()> callback=nullptr);
  static void DisconnectFromServer();

  static void CreateServer(const char* address);
  static void CloseServer();

  static bool ClientIsConnected();
  static bool ServerIsRunning();

 private:
  static class NetworkingModule* networkingModule;

  friend class NetworkingModule;
};
}