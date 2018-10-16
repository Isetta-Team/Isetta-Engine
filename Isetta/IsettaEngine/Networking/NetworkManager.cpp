/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkManager.h"
#include "Core/Config/Config.h"

namespace Isetta {

NetworkingModule* NetworkManager::networkingModule{nullptr};

yojimbo::Message* NetworkManager::GenerateMessageFromClient(
    const char* messageString) {
  return networkingModule->client->CreateMessage(
      NetworkRegistry::GetMessageTypeId(messageString));
}

yojimbo::Message* NetworkManager::GenerateMessageFromServer(
    int clientIdx, const char* messageString) {
  return networkingModule->server->CreateMessage(
      clientIdx, NetworkRegistry::GetMessageTypeId(messageString));
}

void NetworkManager::SendMessageFromClient(yojimbo::Message* message) {
  networkingModule->AddClientToServerMessage(message);
}

void NetworkManager::SendMessageFromServer(int clientIdx,
                                           yojimbo::Message* message) {
  networkingModule->AddServerToClientMessage(clientIdx, message);
}

void NetworkManager::SendAllMessageFromServer(const char tag[NETWORK_TAG_LEN],
                                              yojimbo::Message* refMessage) {
  for (int i = 0; i < GetMaxClients(); i++) {
    if (!networkingModule->server->IsClientConnected(i)) {
      continue;
    }

    yojimbo::Message* newMessage =
        NetworkManager::GenerateMessageFromServer(i, tag);
    newMessage->Copy(refMessage);
    NetworkManager::SendMessageFromServer(i, newMessage);
  }
}

void NetworkManager::ConnectToServer(const char* serverAddress,
                                     Action<bool> callback) {
  networkingModule->Connect(
      serverAddress, Config::Instance().networkConfig.serverPort.GetVal(),
      callback);
}

void NetworkManager::DisconnectFromServer() { networkingModule->Disconnect(); }

void NetworkManager::CreateServer(const char* address) {
  networkingModule->CreateServer(
      address, Config::Instance().networkConfig.serverPort.GetVal());
}

bool NetworkManager::ClientIsConnected() {
  return networkingModule->client->IsConnected();
}

bool NetworkManager::ServerIsRunning() {
  return networkingModule->server && networkingModule->server->IsRunning();
}

int NetworkManager::GetMaxClients() {
  return !ServerIsRunning() ? -1 : networkingModule->server->GetMaxClients();
}

void NetworkManager::CloseServer() { networkingModule->CloseServer(); }
}  // namespace Isetta