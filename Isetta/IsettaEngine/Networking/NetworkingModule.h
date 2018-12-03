/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <functional>
#include "Core/Config/CVar.h"
#include "Core/DataStructures/Delegate.h"
#include "Core/DataStructures/RingBuffer.h"
#include "Core/Time/Clock.h"
#include "Networking/ClientInfo.h"
#include "Networking/Messages.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {
/**
 * @brief Controls the networking side of the engine, including both client and
 * server logic. Uses yojimbo, netcode.io, and reliable.io. Any networking for
 * engine or game is done through this module via the NetworkManager class.
 */
class NetworkingModule {
 public:
  /**
   * @brief Networking configuration CVar struct
   *
   */
  struct NetworkConfig {
    CVarString ipPrefix{"ip_prefix", "128"};
    /// Default client IP address
    CVarString defaultClientIP{"default_client_ip", "0.0.0.0"};
    /// Default server IP address
    CVarString defaultServerIP{"default_server_ip", "127.0.0.1"};
    /// Connects the local client to the remote server
    CVar<int> connectToServer{"connect_to_server", 1};
    /// Runs a local server
    CVar<int> runServer{"run_server", 0};
    /// Default port to use when creating a client.
    CVar<U16> clientPort{"client_port", 30000};
    /// Default port to use when creating a server.
    CVar<U16> serverPort{"server_port", 40000};
    /// Number of bytes used for a private key for the server.
    CVar<int> keyBytes{"key_bytes", 32};
    /// Maximum number of clients the server will support.
    CVar<int> maxClients{"max_clients", 4};
    /// Number of messages the client can have in its send queue before messages
    /// will be overwritten.
    CVar<int> clientQueueSize{"client_queue_size", 256};
    /// Number of messages the server can have in its send queue to an
    /// individual client before messages will be overwritten.
    CVar<int> serverQueueSizePerClient{"server_queue_size_per_client", 256};
    /// Number of possible networked IDs
    CVar<int> maxNetID{"max_network_id", 65000};
    /// Timeout for client disconnect
    CVar<int> timeout{"network_timeout", 20};
  };

 private:
  static CustomAdapter NetworkAdapter;

  /// Keeps time for the client and server. Mainly used for timeouts.
  Clock clock;

  /// Configuration data for both the network and the client. This should
  /// probably stay the same among connected clients and servers.
  yojimbo::ClientServerConfig yojimboConfig;

  /// Key used to join the server.
  U8* privateKey;

  // ------------------- Server Stuff -------------------
  /// Local server's current address and port.
  yojimbo::Address serverAddress;
  /// Local server.
  yojimbo::Server* server;
  NetworkAllocator* serverAllocator;
  /// Queue of messages to be sent from the local server in the next network
  /// update.
  RingBuffer<yojimbo::Message*>* serverSendBufferArray;
  ClientInfo* clientInfos;
  Delegate<ClientInfo> onClientConnected;
  Delegate<ClientInfo> onClientDisconnected;
  bool* wasClientConnectedLastFrame;
  int clientConnectedCallbackHandle;

  // ------------------- Client Stuff -------------------
  /// Local client's current address and port.
  yojimbo::Address clientAddress;
  /// Local client.
  yojimbo::Client* client;
  /// TODO(Caleb): Figure out how to allocate server at runtime instead of at
  /// startup
  NetworkAllocator* clientAllocator;
  /// Identifier for the client on its remote server (might be unused).
  U64 clientId;
  /// Queue of messages to be sent from the local client in the next network
  /// update.
  RingBuffer<yojimbo::Message*>* clientSendBuffer;
  Delegate<> onConnectedToServer;
  Delegate<> onDisconnectedFromServer;
  bool wasClientRunningLastFrame;
  int loadLevelCallbackHandle;

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
  void AddClientToServerMessage(yojimbo::Message* message) const;
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
  void SendClientToServerMessages() const;
  /**
   * @brief Sends the local Server's queued messages for the given client.
   *
   * @param clientIdx Index of the client who will receive the sent messages.
   */
  void SendServerToClientMessages(int clientIdx) const;
  /**
   * @brief Receives the remote Client's messages as packets, constructs them
   * into Message objects, then handles them depending on their type and data.
   *
   * @param clientIdx Index of the client who sent the received messages.
   */
  void ProcessClientToServerMessages(int clientIdx) const;
  /**
   * @brief Receives the remote Server's messages as packets, constructs them
   * into Message objects, then handles them depending on their type and data.
   *
   */
  void ProcessServerToClientMessages() const;

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
               Action<NetworkManager::ClientState> callback = nullptr);
  /**
   * @brief Disconnects the local Client from its connected server, or throws an
   * exception if the Client is not already connected to a server.
   *
   */
  void Disconnect() const;

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
  bool IsClient() const;
  bool IsHost() const;
  bool IsServer() const;

  bool IsClientRunning() const;
  bool IsServerRunning() const;
  bool IsClientConnected(int clientIndex) const;

  friend class NetworkManager;
  friend class EngineLoop;
  friend class StackAllocator;
};
}  // namespace Isetta
