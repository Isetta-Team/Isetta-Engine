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

  /**
   * @brief Enum that defines the current client connection state
   *
   */
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

  /**
   * @brief Populates a message using a
   * lambda function and sends it from the client to the server.
   *
   * @tparam T Class of the message to be sent
   * @param messageInitializer Lambda function that takes a message pointer and
   * populates its data
   */
  template <typename T>
  void SendMessageFromClient(Action<T*> messageInitializer);
  /**
   * @brief Populates a message using a
   * lambda function and sends it from the server to a client.
   *
   * @tparam T Class of the message to be sent
   * @param clientIndex Index of the client that we are sending the message to
   * @param messageInitializer Lambda function that takes a message pointer and
   * populates its data
   */
  template <typename T>
  void SendMessageFromServer(int clientIndex, Action<T*> messageInitializer);

  /**
   * @brief Populates a message using a
   * lambda function and sends it from the server to all clients.
   *
   * @tparam T Class of the message to be sent
   * @param messageInitializer Lambda function that takes a message pointer and
   * populates its data
   */
  template <typename T>
  void SendMessageFromServerToAll(Action<T*> messageInitializer);

  // The following two functions are called when the user already have a message
  /**
   * @brief Populates a message using a reference message and sends it from the
   * server to all clients.
   *
   * @tparam T Class of the message to be sent
   * @param refMessage Message to populate our own message with
   */
  template <typename T>
  void SendMessageFromServerToAll(yojimbo::Message* refMessage);
  /**
   * @brief Populates a message using a reference message and sends it from the
   * server to all clients but the client at the given index.
   *
   * @tparam T Class of the message to be sent
   * @param clientIdx Index of the client that we are sending the message to
   * @param refMessage Message to populate our own message with
   */
  template <typename T>
  void SendMessageFromServerToAllButClient(int clientIdx,
                                           yojimbo::Message* refMessage);

  /**
   * @brief Registers a callback function on the server for when we receive a
   * message.
   *
   * @tparam T Class of the message to run the callback for
   * @param func Function to run when the message is received
   * @return int Handle of the function in the registry to unregister with
   */
  template <typename T>
  int RegisterServerCallback(Action<int, yojimbo::Message*> func);
  /**
   * @brief Unregisters a callback function on the server corresponding to the
   * handle.
   *
   * @tparam T Class of the message corresponding to the callback we want to
   * remove
   * @param handle Handle of the callback function in the registry that we want
   * to remove
   */
  template <typename T>
  void UnregisterServerCallback(int handle);
  /**
   * @brief Registers a callback function on the client for when we receive a
   * message.
   *
   * @tparam T Class of the message to run the callback for
   * @param func Function to run when the message is received
   * @return int Handle of the function in the registry to unregister with
   */
  template <typename T>
  int RegisterClientCallback(Action<yojimbo::Message*> func);
  /**
   * @brief Unregisters a callback function on the client corresponding to the
   * handle.
   *
   * @tparam T Class of the message corresponding to the callback we want to
   * remove
   * @param handle Handle of the callback function in the registry that we want
   * to remove
   */
  template <typename T>
  void UnregisterClientCallback(int handle);

  /**
   * @brief Get the entity corresonding to the given network ID
   *
   * @param id The network ID of the entity that we want
   * @return Entity* A pointer to the entity that we want; null if no such
   * entity exists
   */
  Entity* GetNetworkEntity(const U32 id);
  /**
   * @brief Get the NetworkID component corresponding to the given network ID
   *
   * @param id The network ID of the component that we want
   * @return NetworkId* A pointer to the component that we want; null if no such
   * component exists
   */
  NetworkId* GetNetworkId(const U32 id);

  /**
   * @brief Initializes the local Server object with the given address and port.
   *
   * @param serverIP Address of the server.
   */
  void StartServer(std::string_view serverIP) const;
  /**
   * @brief Closes the local Server object and deallocates its allocated memory.
   *
   */
  void StopServer() const;
  /**
   * @brief Connects the local Client to a server at the given address.
   *
   * @param serverIP Address of the server to connect the local Client to.
   * @param onStarted Function to call when the connection request either
   * succeeds or fails. Passes a boolean indicating the success of the
   * connection.
   */
  void StartClient(std::string_view serverIP,
                   const Action<ClientState>& onStarted = nullptr) const;
  /**
   * @brief Disconnects the local Client from the server it is connected to.
   */
  void StopClient() const;
  /**
   * @brief Starts the server then starts a client and hooks it into it.
   *
   * @param hostIP The IP of the computer to host the server from
   */
  void StartHost(std::string_view hostIP) const;
  /**
   * @brief Disconnects the client then stops the server.
   *
   */
  void StopHost() const;

  // Following three checks "pure role". i.e. a Host is not a Server
  bool IsClient() const;
  bool IsHost() const;
  bool IsServer() const;

  /**
   * @brief Can only be run on the server, and tells all of the clients to load
   * the given level.
   *
   * @param levelName Name of the level to be loaded by the clients
   */
  void NetworkLoadLevel(std::string_view levelName);

  bool IsClientRunning() const;
  bool IsServerRunning() const;
  bool IsClientConnected(int clientIdx) const;

  static int GetMaxClients();
  int GetClientIndex() const;

  ~NetworkManager() = default;

  /**
   * @brief Registers a callback function that is called when the client
   * connects to the server.
   *
   * @param listener Callback function
   * @return U64 Handle of the callback function in the registry
   */
  U64 RegisterConnectedToServerCallback(const Action<>& listener) const;
  /**
   * @brief Unregisters the corresponding callback function on the client for
   * connecting to the server.
   *
   * @param handle Handle of the callback function to unregister
   */
  void UnregisterConnectedToServerCallback(U64& handle) const;
  /**
   * @brief Registers a callback function that is called when the client
   * disconnects from the server.
   *
   * @param listener Callback function
   * @return U64 Handle of the callback function in the registry
   */
  U64 RegisterDisconnectedFromServerCallback(const Action<>& listener) const;
  /**
   * @brief Unregisters the corresponding callback function on the client for
   * disconnecting from the server.
   *
   * @param handle Handle of the callback function to unregister
   */
  void UnregisterDisconnectedFromServerCallback(U64& handle) const;
  /**
   * @brief Registers a callback function that is called when the server gets a
   * client connected to it.
   *
   * @param listener Callback function
   * @return U64 Handle of the callback function in the registry
   */
  U64 RegisterClientConnectedCallback(const Action<ClientInfo>& listener) const;
  /**
   * @brief Unregisters the corresponding callback function on the server for
   * connecting to the server.
   *
   * @param handle Handle of the callback function to unregister
   */
  void UnregisterClientConnectedCallback(U64& handle) const;
  /**
   * @brief Registers a callback function that is called when the server gets a
   * client disconnected from it.
   *
   * @param listener Callback function
   * @return U64 Handle of the callback function in the registry
   */
  U64 RegisterClientDisconnectedCallback(
      const Action<ClientInfo>& listener) const;
  /**
   * @brief Unregisters the corresponding callback function on the server for
   * disconnecting from the server.
   *
   * @param handle Handle of the callback function to unregister
   */
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
