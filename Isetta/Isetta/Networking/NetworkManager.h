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
  /**
   * @brief Sends a HandleMessage from the local Client to its connected server.
   *
   * @param handle Integer value to be sent.
   */
  static void SendHandleMessageFromClient(int handle);
  /**
   * @brief Sends a StringMessage from the local Client to its connected server.
   *
   * @param string String value to be sent. Currently limited to 511 characters.
   */
  static void SendStringMessageFromClient(std::string string);

  /**
   * @brief Sends a HandleMessage from the local Server to the given client.
   *
   * @param clientIdx Index of the client to send the message to.
   * @param handle Integer value to be sent.
   */
  static void SendHandleMessageFromServer(int clientIdx, int handle);
  /**
   * @brief Sends a StringMessage from the local Server to the given client.
   *
   * @param clientIdx Index of the client to send the message to.
   * @param string String value to be sent. Currently limited to 511 characters.
   */
  static void SendStringMessageFromServer(int clientIdx, std::string string);

  /**
   * @brief Connects the local Client to a server at the given address.
   *
   * @param serverAddress Address of the server to connect the local Client to.
   * @param callback Function to call when the connection request either
   * succeeds or fails. Passes a boolean indicating the success of the
   * connection.
   */
  static void ConnectToServer(const char* serverAddress,
                              std::function<void(bool)> callback = nullptr);
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