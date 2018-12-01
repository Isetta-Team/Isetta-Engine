/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <list>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "Core/DataStructures/HandleBin.h"
#include "Core/Debug/Logger.h"
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

  enum class ClientState {
    ConnectTokenExpired = -6,
    InvalidConnectToken = -5,
    ConnectionTimedOut = -4,
    ConnectionResponseTimedOut = -3,
    ConnectionRequestTimedOut = -2,
    ConnectionDenied = -1,
    SendingConnectionRequest = 1,
    SendingConnectionResponse = 2,
    Connected = 3,
  };

  template <typename T>
  void SendMessageFromClient(Action<T*> messageInitializer);
  template <typename T>
  void SendMessageFromServer(int clientIndex, Action<T*> messageInitializer);

  template <typename T>
  void SendMessageFromServerToAll(Action<T*> messageInitializer);

  // The following two are called when the user already have a message
  template <typename T>
  void SendMessageFromServerToAll(yojimbo::Message* refMessage);
  template <typename T>
  void SendMessageFromServerToAllButClient(int clientIdx,
                                           yojimbo::Message* refMessage);

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
                   const Action<ClientState>& onStarted = nullptr) const;
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

  U64 RegisterConnectedToServerCallback(const Action<>& listener) const;
  void UnregisterConnectedToServerCallback(U64& handle) const;
  U64 RegisterDisconnectedFromServerCallback(const Action<>& listener) const;
  void UnregisterDisconnectedFromServerCallback(U64& handle) const;
  U64 RegisterClientConnectedCallback(const Action<ClientInfo>& listener) const;
  void UnregisterClientConnectedCallback(U64& handle) const;
  U64 RegisterClientDisconnectedCallback(const Action<ClientInfo>& listener) const;
  void UnregisterClientDisconnectedCallback(U64& handle) const;

 private:
  NetworkManager();

  template <typename T>
  T* GenerateMessageFromServer(int clientIdx);
  void SendMessageFromServer(int clientIdx, yojimbo::Message* message) const;

  template <typename T>
  T* GenerateMessageFromClient();
  void SendMessageFromClient(yojimbo::Message* message) const;

  template <typename T>
  bool RegisterMessageType(U64 size, Func<yojimbo::Message*, void*> factory);
  U16 GetMessageTypeCount() const { return messageTypeCount; }
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
  friend class CustomAdapter;
  template <typename T>
  friend class NetworkMessageRegistry;
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
void NetworkManager::SendMessageFromClient(Action<T*> messageInitializer) {
  yojimbo::Message* newMessage = GenerateMessageFromClient<T>();
  messageInitializer(reinterpret_cast<T*>(newMessage));
  SendMessageFromClient(newMessage);
}

template <typename T>
void NetworkManager::SendMessageFromServer(const int clientIndex,
                                           Action<T*> messageInitializer) {
  yojimbo::Message* newMessage = GenerateMessageFromServer<T>(clientIndex);
  messageInitializer(reinterpret_cast<T*>(newMessage));
  SendMessageFromServer(clientIndex, newMessage);
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
void NetworkManager::SendMessageFromServerToAll(Action<T*> messageInitializer) {
  if (!IsServerRunning()) {
    LOG_ERROR(Debug::Channel::Networking,
              "Server is not running, cannot SendMessageFromServer");
    return;
  }

  for (int i = 0; i < GetMaxClients(); ++i) {
    if (!IsClientConnected(i)) {
      continue;
    }

    yojimbo::Message* newMessage = GenerateMessageFromServer<T>(i);
    messageInitializer(reinterpret_cast<T*>(newMessage));
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
      [handle](const std::pair<U16, Action<U16, yojimbo::Message*>> item) {
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
      [handle](const std::pair<U16, Action<yojimbo::Message*>> item) {
        return item.first == handle;
      });
}

}  // namespace Isetta
