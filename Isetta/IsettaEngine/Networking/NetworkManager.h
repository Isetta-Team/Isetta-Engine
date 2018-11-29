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

  yojimbo::Message* CreateClientMessage(int messageId) const;
  yojimbo::Message* CreateServerMessage(int clientIdx, int messageId) const;

  template <typename T>
  int GetMessageTypeId();
  std::list<std::pair<U16, Action<yojimbo::Message*>>> GetClientFunctions(
      int type);
  std::list<std::pair<U16, Action<int, yojimbo::Message*>>> GetServerFunctions(
      int type);
  U32 CreateNetworkId(NetworkId* networkId);
  U32 AssignNetworkId(U32 netId, NetworkId* networkId);
  void RemoveNetworkId(NetworkId* networkId);
  static U16 GetServerPort();

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
  void SendMessageFromClient(yojimbo::Message* message) const;
  void SendMessageFromServer(int clientIdx, yojimbo::Message* message) const;
  template <typename T>
  void SendAllMessageFromServer(yojimbo::Message* refMessage);
  template <typename T>
  void SendAllButClientMessageFromServer(int clientIdx,
                                         yojimbo::Message* refMessage);

  U16 GetMessageTypeCount() const { return messageTypeCount; }
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
   * \brief Initializes the local Server object with the given address and port.
   *
   * \param serverIP Address of the server.
   */
  void StartServer(const char* serverIP) const;
  void StartServer(const std::string& serverIP) const;
  /**
   * \brief Closes the local Server object and deallocates its allocated memory.
   *
   */
  void StopServer() const;
  /**
   * \brief Connects the local Client to a server at the given address.
   *
   * \param serverIP Address of the server to connect the local Client to.
   * \param onStarted Function to call when the connection request either
   * succeeds or fails. Passes a boolean indicating the success of the
   * connection.
   */
  void StartClient(const char* serverIP,
                   const Action<bool>& onStarted = nullptr) const;
  void StartClient(const std::string& serverIP,
                   const Action<bool>& onStarted = nullptr) const;
  /**
   * \brief Disconnects the local Client from the server it is connected to.
   */
  void StopClient() const;
  void StartHost(const char* hostIP) const;
  void StartHost(const std::string& hostIP) const;
  void StopHost() const;

  bool LocalClientIsConnected() const;
  bool ClientIsConnected(int clientIdx) const;
  bool ServerIsRunning() const;
  static int GetMaxClients();
  int GetClientIndex() const;

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
