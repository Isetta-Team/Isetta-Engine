/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkManager.h"
#include "Core/Config/Config.h"
#include "Networking/BuiltinMessages.h"
#include "Networking/NetworkId.h"
#include "Networking/NetworkTransform.h"
#include "Networking/NetworkingModule.h"
#include "Scene/Entity.h"

namespace Isetta {

NetworkManager::NetworkManager() : networkIds(1) {}

NetworkManager& NetworkManager::Instance() {
  static NetworkManager instance;
  return instance;
}

void NetworkManager::SendMessageFromClient(yojimbo::Message* message) const {
  networkingModule->AddClientToServerMessage(message);
}

void NetworkManager::SendMessageFromServer(const int clientIdx,
                                           yojimbo::Message* message) const {
  networkingModule->AddServerToClientMessage(clientIdx, message);
}

bool NetworkManager::IsClientRunning() const {
  return networkingModule->IsClientRunning();
}

bool NetworkManager::IsServerRunning() const {
  return networkingModule->IsServerRunning();
}

bool NetworkManager::IsClientConnected(const int clientIdx) const {
  return networkingModule->IsClientConnected(clientIdx);
}

int NetworkManager::GetMaxClients() {
  return CONFIG_VAL(networkConfig.maxClients);
}

int NetworkManager::GetClientIndex() const {
  return networkingModule->client->GetClientIndex();
}

U64 NetworkManager::RegisterConnectedToServerCallback(
    const Action<>& listener) const {
  return networkingModule->onConnectedToServer.Subscribe(listener);
}

void NetworkManager::UnregisterConnectedToServerCallback(U64& handle) const {
  networkingModule->onConnectedToServer.Unsubscribe(handle);
}

U64 NetworkManager::RegisterDisconnectedFromServerCallback(
    const Action<>& listener) const {
  return networkingModule->onDisconnectedFromServer.Subscribe(listener);
}

void NetworkManager::UnregisterDisconnectedFromServerCallback(U64& handle) const {
  networkingModule->onDisconnectedFromServer.Unsubscribe(handle);
}

U64 NetworkManager::RegisterClientConnectedCallback(
    const Action<ClientInfo>& listener) const {
  return networkingModule->onClientConnected.Subscribe(listener);
}

void NetworkManager::UnregisterClientConnectedCallback(U64& handle) const {
  networkingModule->onClientConnected.Unsubscribe(handle);
}

U64 NetworkManager::RegisterClientDisconnectedCallback(
    const Action<ClientInfo>& listener) const {
  return networkingModule->onClientDisconnected.Subscribe(listener);
}

void NetworkManager::UnregisterClientDisconnectedCallback(U64& handle) const {
  networkingModule->onClientDisconnected.Unsubscribe(handle);
}

void NetworkManager::StartServer(std::string_view serverIP) const {
  networkingModule->CreateServer(serverIP.data(), GetServerPort());
}

void NetworkManager::StopServer() const { networkingModule->CloseServer(); }

void NetworkManager::StartClient(std::string_view serverIP,
                                 const Action<ClientState>& onStarted) const {
  networkingModule->Connect(serverIP.data(), GetServerPort(), onStarted);
}

void NetworkManager::StopClient() const { networkingModule->Disconnect(); }

void NetworkManager::StartHost(std::string_view hostIP) const {
  networkingModule->CreateServer(hostIP.data(), GetServerPort());
  networkingModule->Connect(hostIP.data(), GetServerPort());
  LOG_INFO(Debug::Channel::Networking, "Host Started on %s", hostIP.data());
}

void NetworkManager::StopHost() const {
  networkingModule->Disconnect();
  networkingModule->CloseServer();
}

bool NetworkManager::IsClient() const { return networkingModule->IsClient(); }

bool NetworkManager::IsHost() const { return networkingModule->IsHost(); }

bool NetworkManager::IsServer() const { return networkingModule->IsServer(); }

void NetworkManager::NetworkLoadLevel(std::string_view levelName) {
  if (!IsServerRunning()) {
    LOG_ERROR(Debug::Channel::Networking,
              "NetworkLevel can only be called when server running");
    return;
  }
  if (levelName.length() >= LoadLevelMessage::levelNameMaxLength) {
    LOG_ERROR(Debug::Channel::Networking,
              "Level name [%s] is too long and cannot be sent over network, "
              "consider giving it a short name",
              levelName.data());
    return;
  }
  SendMessageFromServerToAll<LoadLevelMessage>(
      [levelName](LoadLevelMessage* inMessage) {
        strcpy_s(inMessage->levelName, levelName.data());
      });

  if (!IsClientRunning()) {
    LevelManager::Instance().LoadLevel(levelName);
  }
}

std::list<std::pair<U16, Action<yojimbo::Message*>>>
NetworkManager::GetClientFunctions(const int type) {
  return clientCallbacks[type];
}
std::list<std::pair<U16, Action<int, yojimbo::Message*>>>
NetworkManager::GetServerFunctions(const int type) {
  return serverCallbacks[type];
}

yojimbo::Message* NetworkManager::CreateClientMessage(
    const int messageId) const {
  return networkingModule->client->CreateMessage(messageId);
}

yojimbo::Message* NetworkManager::CreateServerMessage(
    const int clientIdx, const int messageId) const {
  return networkingModule->server->CreateMessage(clientIdx, messageId);
}

Entity* NetworkManager::GetNetworkEntity(const U32 id) {
  auto it = networkIdToComponentMap.find(id);
  if (it != networkIdToComponentMap.end()) {
    return it->second->entity;
  }
  return nullptr;
}

NetworkId* NetworkManager::GetNetworkId(const U32 id) {
  auto it = networkIdToComponentMap.find(id);
  if (it != networkIdToComponentMap.end()) {
    return it->second;
  }
  return nullptr;
}

U32 NetworkManager::CreateNetworkId(NetworkId* networkId) {
  if (!IsServerRunning()) {
    throw std::exception("Cannot create a new network id on a client");
  }
  U32 netId = networkIds.GetHandle();
  networkId->id = netId;
  networkIdToComponentMap[netId] = networkId;
  NetworkTransform::serverPosTimestamps[netId] = 0;
  NetworkTransform::serverRotTimestamps[netId] = 0;
  NetworkTransform::serverScaleTimestamps[netId] = 0;
}

U32 NetworkManager::AssignNetworkId(U32 netId, NetworkId* networkId) {
  if (networkIdToComponentMap.find(netId) != networkIdToComponentMap.end()) {
    throw std::exception(Util::StrFormat(
        "Multiple objects trying to assign to the same network id: %d", netId));
  } else if (networkId->id > 0) {
    throw std::exception(
        Util::StrFormat("Trying to assign network id %d to existing network "
                        "object with id %d",
                        netId, networkId->id));
  }
  networkId->id = netId;
  networkIdToComponentMap[netId] = networkId;
  networkIds.RemoveHandle(netId);
}

void NetworkManager::RemoveNetworkId(NetworkId* networkId) {
  if (!networkId->id) {
    throw std::exception(Util::StrFormat(
        "Cannot remove network id on a nonexistent network object"));
  }
  networkIdToComponentMap.erase(networkId->id);
  networkIds.ReturnHandle(networkId->id);
  networkId->id = NULL;
}

inline U16 NetworkManager::GetServerPort() {
  return CONFIG_VAL(networkConfig.serverPort);
}
}  // namespace Isetta
