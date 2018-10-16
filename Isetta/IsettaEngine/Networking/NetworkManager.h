/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Core/IsettaAlias.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

#define NETWORK_TAG_LEN 5
class Entity;
class NetworkIdentity;

/**
 * @brief Wrapper class for NetworkingModule so that other engine components can
 * use networking features.
 */
class NetworkManager {
 public:
  static yojimbo::Message* GenerateMessageFromClient(const char* messageString);
  static yojimbo::Message* GenerateMessageFromServer(int clientIdx,
                                                     const char* messageString);
  // TODO(Caleb) Consider merging the generate and send functions
  static void SendMessageFromClient(yojimbo::Message* message);
  static void SendMessageFromServer(int clientIdx, yojimbo::Message* message);
  static void SendAllMessageFromServer(const char tag[NETWORK_TAG_LEN],
                                       yojimbo::Message* message);

  static U16 GetMessageTypeCount() { return messageTypeCount; }
  static void RegisterMessageType(U64 size,
                                  Func<yojimbo::Message*, void*> factory,
                                  const char tag[NETWORK_TAG_LEN]);
  static int RegisterServerCallback(const char tag[NETWORK_TAG_LEN],
                                    Action<int, yojimbo::Message*> func);
  static void UnregisterServerCallback(const char tag[NETWORK_TAG_LEN],
                                       int handle);
  static int RegisterClientCallback(const char tag[NETWORK_TAG_LEN],
                                    Action<yojimbo::Message*> func);
  static void UnregisterClientCallback(const char tag[NETWORK_TAG_LEN],
                                       int handle);

  static Entity* GetNetworkEntity(const U32 id);
  static U32 CreateNetworkId(NetworkIdentity* networkIdentity);
  static U32 AssignNetworkId(U32 netId, NetworkIdentity* networkIdentity);
  static void RemoveNetworkId(NetworkIdentity* networkIdentity);

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
  static int GetMessageTypeId(const char tag[NETWORK_TAG_LEN]);
  static std::list<std::pair<U16, Action<yojimbo::Message*>>>
  GetClientFunctions(int type);
  static std::list<std::pair<U16, Action<int, yojimbo::Message*>>>
  GetServerFunctions(int type);

  static class NetworkingModule* networkingModule;

  static int messageTypeCount;
  static U16 functionCount;
  static U32 nextNetworkId;
  static std::unordered_map<int, std::pair<U64, Func<yojimbo::Message*, void*>>>
      factories;
  static std::unordered_map<const char*, int> tags;

  static std::unordered_map<
      int, std::list<std::pair<U16, Action<yojimbo::Message*>>>>
      clientCallbacks;
  static std::unordered_map<
      int, std::list<std::pair<U16, Action<int, yojimbo::Message*>>>>
      serverCallbacks;

  static std::unordered_map<U32, Entity*> networkIdToEntityMap;

  friend class NetworkingModule;
  friend class NetworkMessageFactory;
};
}  // namespace Isetta