/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Networking/NetworkingModule.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {
/**
 * @brief Wrapper class for NetworkingModule so that other engine components can
 * use networking features.
 */
class NetworkManager {
 public:
  static yojimbo::Message* GenerateMessageFromClient(const char* messageString);
  static yojimbo::Message* GenerateMessageFromServer(int clientIdx, const char* messageString);
  // TODO(Caleb) Consider merging the generate and send functions
  static void SendMessageFromClient(yojimbo::Message* message);
  static void SendMessageFromServer(int clientIdx, yojimbo::Message* message);

  /**
   * @brief Connects the local Client to a server at the given address.
   *
   * @param serverAddress Address of the server to connect the local Client to.
   * @param callback Function to call when the connection request either
   * succeeds or fails. Passes a boolean indicating the success of the
   * connection.
   */
  static void ConnectToServer(const char* serverAddress,
                              Action<bool> callback = nullptr);
  /**
   * @brief Disconnects the local Client from the server it is connected to.
   *
   */
  static void DisconnectFromServer();

  /**
   * @brief Initializes the local Server object with the given address and port.
   *
   * @param address Address of the server.
   */
  static void CreateServer(const char* address);
  /**
   * @brief Closes the local Server object and deallocates its allocated memory.
   *
   */
  static void CloseServer();

  static bool ClientIsConnected();
  static bool ServerIsRunning();
  static int GetMaxClients();

 private:
  static class NetworkingModule* networkingModule;

  friend class NetworkingModule;
};
}  // namespace Isetta