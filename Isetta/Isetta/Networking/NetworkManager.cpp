/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkManager.h"

namespace Isetta {

NetworkingModule* NetworkManager::networkingModule{nullptr};

void NetworkManager::SendHandleMessageFromClient(int handle) {
  HandleMessage* message = static_cast<HandleMessage*>(
      networkingModule->client->CreateMessage(HANDLE_MESSAGE));
  message->handle = handle;
  networkingModule->AddClientToServerMessage(message);
}
void NetworkManager::SendStringMessageFromClient(std::string string) {
  StringMessage* message = static_cast<StringMessage*>(
      networkingModule->client->CreateMessage(STRING_MESSAGE));
  message->string = string;
  networkingModule->AddClientToServerMessage(message);
}

void NetworkManager::SendHandleMessageFromServer(int clientIdx, int handle) {
  HandleMessage* message = static_cast<HandleMessage*>(
      networkingModule->server->CreateMessage(clientIdx, HANDLE_MESSAGE));
  message->handle = handle;
  networkingModule->AddServerToClientMessage(clientIdx, message);
}
void NetworkManager::SendStringMessageFromServer(int clientIdx, std::string string) {
  StringMessage* message = static_cast<StringMessage*>(
      networkingModule->server->CreateMessage(clientIdx, STRING_MESSAGE));
  message->string = string;
  networkingModule->AddServerToClientMessage(clientIdx, message);
}


void NetworkManager::ConnectToServer(const char* serverAddress,
                                      std::function<void(bool)> callback) {
  networkingModule->Connect(serverAddress, networkingModule->ServerPort, callback);
}

void NetworkManager::DisconnectFromServer() {
  networkingModule->Disconnect();
}

void NetworkManager::CreateServer(const char* address) {
  networkingModule->CreateServer(address, networkingModule->ServerPort);
}

bool NetworkManager::ClientIsConnected() {
  return networkingModule->client->IsConnected();
}

bool NetworkManager::ServerIsRunning() {
  return networkingModule->server->IsRunning();
}

void NetworkManager::CloseServer() {
  networkingModule->CloseServer();
}
}