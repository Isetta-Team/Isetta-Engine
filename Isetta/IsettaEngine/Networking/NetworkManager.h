/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <list>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "Core/IsettaAlias.h"
#include "yojimbo/yojimbo.h"

namespace Isetta {

class Entity;
class NetworkIdentity;

/**
 * @brief Wrapper class for NetworkingModule so that other engine components can
 * use networking features.
 */
class NetworkManager {
 public:
  template <typename T>
  static T* GenerateMessageFromClient();
  template <typename T>
  static T* GenerateMessageFromServer(int clientIdx);
  // TODO(Caleb) Consider merging the generate and send functions
  static void SendMessageFromClient(yojimbo::Message* message);
  static void SendMessageFromServer(int clientIdx, yojimbo::Message* message);
  template <typename T>
  static void SendAllMessageFromServer(yojimbo::Message* message);

  static U16 GetMessageTypeCount() { return messageTypeCount; }
  template <typename T>
  static void RegisterMessageType(U64 size,
                                  Func<yojimbo::Message*, void*> factory);
  template <typename T>
  static int RegisterServerCallback(Action<int, yojimbo::Message*> func);
  template <typename T>
  static void UnregisterServerCallback(int handle);
  template <typename T>
  static int RegisterClientCallback(Action<yojimbo::Message*> func);
  template <typename T>
  static void UnregisterClientCallback(int handle);

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

  static bool LocalClientIsConnected();
  static bool ClientIsConnected(int clientIdx);
  static bool ServerIsRunning();
  static int GetMaxClients();

 private:
  static yojimbo::Message* CreateClientMessage(int messageId);
  static yojimbo::Message* CreateServerMessage(int clientIdx, int messageId);

  template <typename T>
  static int GetMessageTypeId();
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
  static std::unordered_map<std::type_index, int> typeMap;

  static std::unordered_map<
      int, std::list<std::pair<U16, Action<yojimbo::Message*>>>>
      clientCallbacks;
  static std::unordered_map<
      int, std::list<std::pair<U16, Action<int, yojimbo::Message*>>>>
      serverCallbacks;

  static std::unordered_map<U32, NetworkIdentity*> networkIdToComponentMap;

  friend class NetworkingModule;
  friend class NetworkMessageFactory;
};

template <typename T>
T* NetworkManager::GenerateMessageFromClient() {
  return reinterpret_cast<T*>(CreateClientMessage(GetMessageTypeId<T>()));
}
template <typename T>
T* NetworkManager::GenerateMessageFromServer(int clientIdx) {
  return reinterpret_cast<T*>(CreateServerMessage(clientIdx, GetMessageTypeId<T>()));
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
void NetworkManager::RegisterMessageType(
    U64 size, Func<yojimbo::Message*, void*> factory) {
  factories[messageTypeCount] = std::pair(size, factory);
  typeMap[std::type_index(typeid(T))] = messageTypeCount;
  ++messageTypeCount;
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
