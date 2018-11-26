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
#include "Networking/ClientInfo.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

class Entity;
class NetworkId;
/**
 * @brief Wrapper class for NetworkingModule so that other engine components can
 * use networking features.
 */
class ISETTA_API_DECLARE NetworkManager {
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
  void SendMessageFromServerToAll(yojimbo::Message* refMessage);
  template <typename T>
  void SendMessageFromServerToAllButClient(int clientIdx,
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
  void StartServer(std::string_view serverIP) const;
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
  void StartClient(std::string_view serverIP,
                   const Action<bool>& onStarted = nullptr) const;
  /**
   * \brief Disconnects the local Client from the server it is connected to.
   */
  void StopClient() const;
  void StartHost(std::string_view hostIP) const;
  void StopHost() const;

  // Following three checks "pure role". i.e. a Host is not a Server
  bool IsClient() const;
  bool IsHost() const;
  bool IsServer() const;

  void NetworkLoadLevel(std::string_view levelName);

  bool IsClientRunning() const;
  bool IsServerRunning() const;
  bool IsClientConnected(int clientIdx) const;

  static int GetMaxClients();
  int GetClientIndex() const;

  ~NetworkManager() = default;

  U64 AddConnectedToServerListener(const Action<>& listener) const;
  void RemoveConnectedToServerListener(U64 handle) const;
  U64 AddDisconnectedFromServerListener(const Action<>& listener) const;
  void RemoveDisconnectedFromServerListener(U64 handle) const;
  // int parameter in Action is ClientID
  U64 AddClientConnectedListener(const Action<ClientInfo>& listener) const;
  void RemoveClientConnectedListener(U64 handle) const;
  // int parameter in Action is ClientID
  U64 AddClientDisconnectedListener(const Action<ClientInfo>& listener) const;
  void RemoveClientDisconnectedListener(U64 handle) const;

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

  class NetworkingModule* networkingModule{nullptr};

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
void NetworkManager::SendMessageFromServerToAll(yojimbo::Message* refMessage) {
  for (int i = 0; i < GetMaxClients(); ++i) {
    if (!IsClientConnected(i)) {
      continue;
    }

    yojimbo::Message* newMessage = GenerateMessageFromServer<T>(i);
    newMessage->Copy(refMessage);
    SendMessageFromServer(i, newMessage);
  }
}
template <typename T>
void NetworkManager::SendMessageFromServerToAllButClient(
    int clientIdx, yojimbo::Message* refMessage) {
  for (int i = 0; i < GetMaxClients(); ++i) {
    if (!IsClientConnected(i) || i == clientIdx) {
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
