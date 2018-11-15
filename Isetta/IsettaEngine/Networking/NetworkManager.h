/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <list>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "Core/DataStructures/HandleBin.h"
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"
#include "yojimbo/yojimbo.h"
#include "ISETTA_API.h"

namespace Isetta {

class Entity;
class NetworkId;

/**
 * @brief Wrapper class for NetworkingModule so that other engine components can
 * use networking features.
 */
class ISETTA_API_DECLARE NetworkManager {
 private:
  NetworkManager();

  yojimbo::Message* CreateClientMessage(int messageId);
  yojimbo::Message* CreateServerMessage(int clientIdx, int messageId);

  template <typename T>
  int GetMessageTypeId();
  std::list<std::pair<U16, Action<yojimbo::Message*>>> GetClientFunctions(
      int type);
  std::list<std::pair<U16, Action<int, yojimbo::Message*>>> GetServerFunctions(
      int type);
  U32 CreateNetworkId(NetworkId* NetworkId);
  U32 AssignNetworkId(U32 netId, NetworkId* NetworkId);
  void RemoveNetworkId(NetworkId* NetworkId);

  class NetworkingModule* networkingModule;

  int messageTypeCount = 0;
  U16 functionCount = 0;
  HandleBin networkIds;
  std::unordered_map<int, std::pair<U64, Func<yojimbo::Message*, void*>>>
      factories;
  std::unordered_map<std::type_index, int> typeMap;

  std::unordered_map<int, std::list<std::pair<U16, Action<yojimbo::Message*>>>>
      clientCallbacks;
  std::unordered_map<int,
                     std::list<std::pair<U16, Action<int, yojimbo::Message*>>>>
      serverCallbacks;

  std::unordered_map<U32, NetworkId*> networkIdToComponentMap;

 public:
  static NetworkManager& Instance();

  template <typename T>
  T* GenerateMessageFromClient();
  template <typename T>
  T* GenerateMessageFromServer(int clientIdx);
  // TODO(Caleb) Consider merging the generate and send functions
  void SendMessageFromClient(yojimbo::Message* message);
  void SendMessageFromServer(int clientIdx, yojimbo::Message* message);
  template <typename T>
  void SendAllMessageFromServer(yojimbo::Message* message);
  template <typename T>
  void SendAllButClientMessageFromServer(int clinetIdx,
                                         yojimbo::Message* message);

  U16 GetMessageTypeCount() { return messageTypeCount; }
  template <typename T>
  bool RegisterMessageType(U64 size, Func<yojimbo::Message*, void*> factory);
  template <typename T>
  int RegisterServerCallback(Action<int, yojimbo::Message*> func);
  template <typename T>
  void UnregisterServerCallback(int handle);
  template <typename T>
  int RegisterClientCallback(Action<yojimbo::Message*> func);
  template <typename T>
  void UnregisterClientCallback(int handle);

  Entity* GetNetworkEntity(const U32 id);
  NetworkId* GetNetworkId(const U32 id);

  /**
   * @brief Connects the local Client to a server at the given address.
   *
   * @param serverAddress Address of the server to connect the local Client to.
   * @param callback Function to call when the connection request either
   * succeeds or fails. Passes a boolean indicating the success of the
   * connection.
   */
  void ConnectToServer(const char* serverAddress,
                       Action<bool> callback = nullptr);
  /**
   * @brief Disconnects the local Client from the server it is connected to.
   *
   */
  void DisconnectFromServer();

  /**
   * @brief Initializes the local Server object with the given address and port.
   *
   * @param address Address of the server.
   */
  void CreateServer(const char* address);
  /**
   * @brief Closes the local Server object and deallocates its allocated memory.
   *
   */
  void CloseServer();

  bool LocalClientIsConnected();
  bool ClientIsConnected(int clientIdx);
  bool ServerIsRunning();
  int GetMaxClients();
  int GetClientIndex();

  ~NetworkManager() = default;

  friend class NetworkId;
  friend class NetworkingModule;
  friend class NetworkMessageFactory;
};

template <typename T>
T* NetworkManager::GenerateMessageFromClient() {
  return reinterpret_cast<T*>(CreateClientMessage(GetMessageTypeId<T>()));
}
template <typename T>
T* NetworkManager::GenerateMessageFromServer(int clientIdx) {
  return reinterpret_cast<T*>(
      CreateServerMessage(clientIdx, GetMessageTypeId<T>()));
}
template <typename T>
int NetworkManager::GetMessageTypeId() {
  return typeMap[std::type_index(typeid(T))];
}
template <typename T>
void NetworkManager::SendAllMessageFromServer(yojimbo::Message* refMessage) {
  for (int i = 0; i < GetMaxClients(); ++i) {
    if (!ClientIsConnected(i)) {
      continue;
    }

    yojimbo::Message* newMessage = GenerateMessageFromServer<T>(i);
    newMessage->Copy(refMessage);
    SendMessageFromServer(i, newMessage);
  }
}
template <typename T>
void NetworkManager::SendAllButClientMessageFromServer(
    int clientIdx, yojimbo::Message* refMessage) {
  for (int i = 0; i < GetMaxClients(); ++i) {
    if (!ClientIsConnected(i) || i == clientIdx) {
      continue;
    }

    yojimbo::Message* newMessage = GenerateMessageFromServer<T>(i);
    newMessage->Copy(refMessage);
    SendMessageFromServer(i, newMessage);
  }
}
template <typename T>
bool NetworkManager::RegisterMessageType(
    U64 size, Func<yojimbo::Message*, void*> factory) {
  factories[messageTypeCount] = std::pair(size, factory);
  return typeMap
      .insert_or_assign(std::type_index(typeid(T)), messageTypeCount++)
      .second;
}
template <typename T>
int NetworkManager::RegisterServerCallback(
    Action<int, yojimbo::Message*> func) {
  serverCallbacks[GetMessageTypeId<T>()].push_back(
      std::pair(functionCount, func));
  return functionCount++;
}
template <typename T>
void NetworkManager::UnregisterServerCallback(int handle) {
  int messageId = GetMessageTypeId<T>();
  serverCallbacks[messageId].remove_if(
      [handle](std::pair<U16, Action<U16, yojimbo::Message*>> item) {
        return item.first == handle;
      });
}
template <typename T>
int NetworkManager::RegisterClientCallback(Action<yojimbo::Message*> func) {
  clientCallbacks[GetMessageTypeId<T>()].push_back(
      std::pair(functionCount, func));
  return functionCount++;
}
template <typename T>
void NetworkManager::UnregisterClientCallback(int handle) {
  int messageId = GetMessageTypeId<T>();
  clientCallbacks[messageId].remove_if(
      [handle](std::pair<U16, Action<yojimbo::Message*>> item) {
        return item.first == handle;
      });
}

}  // namespace Isetta
