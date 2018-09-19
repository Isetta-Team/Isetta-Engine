/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <functional>
#include "Core/DataStructures/RingBuffer.h"
#include "Core/Time/Clock.h"
#include "Networking/Messages.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {
/**
 * @brief Controls the networking side of the engine, including both client and
 * server logic. Uses yojimbo, netcode.io, and reliable.io. Any networking for
 * engine or game is done through this module via the NetworkManager class.
 */
class NetworkingModule {
 private:
  static CustomAdapter NetworkAdapter;

  // TODO(Caleb): Look into making the ports user-definable
  /// Default port to use when creating a client.
  const U16 ClientPort = 30000;
  /// Default port to use when creating a server.
  const U16 ServerPort = 40000;
  /// Number of bytes used for a private key for the server.
  const int KeyBytes = 32;
  /// Maximum number of clients the server will support.
  const int MaxClients = 2;
  /// Number of messages the client can have in its send queue before messages
  /// will be overwritten.
  const int ClientQueueSize = 256;
  /// Number of messages the server can have in its send queue to an individual
  /// client before messages will be overwritten.
  const int ServerQueueSizePerClient = 256;

  /// Keeps time for the client and server. Mainly used for timeouts.
  Clock clock;

  /// Local client's current address and port.
  yojimbo::Address clientAddress;
  /// Local client.
  yojimbo::Client* client;
  /// Local server's current address and port.
  yojimbo::Address serverAddress;
  /// Local server.
  yojimbo::Server* server;
  /// Configuration data for both the network and the client. This should
  /// probably stay the same among connected clients and servers.
  yojimbo::ClientServerConfig networkConfig;

  /// Key used to join the server.
  U8* privateKey;
  /// Identifier for the client on its remote server (might be unused).
  U64 clientId;

  /// Queue of messages to be sent from the local client in the next network
  /// update.
  RingBuffer<yojimbo::Message*> clientSendBuffer;
  /// Queue of messages to be sent from the local server in the next network
  /// update.
  RingBuffer<yojimbo::Message*>* serverSendBufferArray;

  // Constructors

  NetworkingModule() = default;
  ~NetworkingModule() = default;

  // Module functions
  /**
   * @brief Initializes the networking module, which includes initializing the
   * yojimbo codebase, building the configuration data, and instantiating the
   * local Client object.
   */
  void StartUp();
  /**
   * @brief Advances the time for both the local Server and Client, then gathers
   * outgoing and incoming packets and send/deserailizes them. No packets or
   * messages are sent outside of this function.
   *
   * @param deltaTime Time since the last update frame.
   */
  void Update(float deltaTime);
  /**
   * @brief Disconnects the local Client and closes the local Server if
   * necessary and deallocates any allocated memory.
   */
  void ShutDown();

  // Functions
  /**
   * @brief Adds the given Message into the local Client's send queue.
   *
   * @param message Pointer to a Message object that should have been created by
   * a MessageFactory.
   */
  void AddClientToServerMessage(yojimbo::Message* message);
  /**
   * @brief Adds the given Message into the local Server's send queue to the
   * given client.
   *
   * @param clientIdx Index of the client who will eventually receive the added
   * messages.
   * @param message Pointer to a Message object that should have been created by
   * a MessageFactory.
   */
  void AddServerToClientMessage(int clientIdx, yojimbo::Message* message);

  /**
   * @brief Checks for connecting clients in the local Server, then sends queued
   * packets, then receives incoming packets. The received packets are
   * constructed into Message objects which are subsequently handled.
   *
   * @param time Current runtime of the client and server. Used for timeouts.
   */
  void PumpClientServerUpdate(double time);

  /**
   * @brief Sends the local Client's queued messages.
   *
   */
  void SendClientToServerMessages();
  /**
   * @brief Sends the local Server's queued messages for the given client.
   *
   * @param clientIdx Index of the client who will receive the sent messages.
   */
  void SendServerToClientMessages(int clientIdx);
  /**
   * @brief Receives the remote Client's messages as packets, constructs them
   * into Message objects, then handles them depending on their type and data.
   *
   * @param clientIdx Index of the client who sent the received messages.
   */
  void ProcessClientToServerMessages(int clientIdx);
  /**
   * @brief Receives the remote Server's messages as packets, constructs them
   * into Message objects, then handles them depending on their type and data.
   *
   */
  void ProcessServerToClientMessages();

  /**
   * @brief Attempts to connect the local Client to a server at the given
   * address and port. Does nothing if the local Client is already connected to
   * a server.
   *
   * @param serverAddress Address of the server to connect to.
   * @param serverPort Port of the server to connect to.
   * @param callback Function to call when the connection request either
   * succeeds or fails. Passes a boolean indicating the success of the
   * connection.
   */
  void Connect(const char* serverAddress, int serverPort,
               std::function<void(bool)> callback = nullptr);
  /**
   * @brief Disconnects the local Client from its connected server, or throws an
   * exception if the Client is not already connected to a server.
   *
   */
  void Disconnect();

  /**
   * @brief Initializes the local Server object with the given address and port.
   * Throws an exception if a server is already running.
   *
   * @param address Address of the server.
   * @param port Port of the server.
   */
  void CreateServer(const char* address, int port);
  /**
   * @brief Closes the local Server object and deallocates its allocated memory.
   * Throws an exception if a server is not running.
   *
   */
  void CloseServer();

  // Other

  friend class NetworkManager;
  friend class ModuleManager;
};
}  // namespace Isetta
