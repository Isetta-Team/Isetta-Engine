/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkingModule.h"

namespace Isetta {
CustomAdapter NetworkingModule::NetworkAdapter;
void NetworkingModule::StartUp() {
  // TODO(Caleb): Add CVar inputs for the config options
  networkConfig.channel[0].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;

  clientAddress = yojimbo::Address("0.0.0.0", NetworkingModule::ClientPort);
  // TODO(Caleb): change out the memory allocation with our own custom allocator
  client =
      new yojimbo::Client(yojimbo::GetDefaultAllocator(), clientAddress,
                               networkConfig, NetworkingModule::NetworkAdapter,
                               clock.GetElapsedTime());

  privateKey = new uint8_t[KeyBytes];
  // TODO(Caleb): Need to do something more insightful with the private key than
  // all 0s
  memset(privateKey, 0, KeyBytes);

  yojimbo::random_bytes((uint8_t*)&clientId, 8);

  clientSendRBuffer = new CustomMessage[ClientQueueSize];
  clientReceiveRBuffer = new CustomMessage[ClientQueueSize];
  serverSendRBuffer = new CustomMessage[ServerQueueSize];
  serverReceiveRBuffer = new CustomMessage[ServerQueueSize];
}

void NetworkingModule::Update() {
  clock.UpdateTime();

  // Check for new connections
  for (int i = 0; i < NumIterations; i++) {
    PumpClientServerUpdate(clock.GetElapsedTime());

    if (client->ConnectionFailed() ||
      (server && !server->IsRunning())) {
      break;
    }

    if (!server && !client->IsConnecting() && client->IsConnected()) {
      break;
    }
  }

  // TODO(Caleb): The rest of the networking
}

void NetworkingModule::ShutDown() {
  // TODO(Caleb): Change the mem dealloc with our new manager
  delete client;
  if (server) {
    delete server;
  }
  delete privateKey;
  delete clientSendRBuffer;
  delete clientReceiveRBuffer;
  delete serverSendRBuffer;
  delete serverReceiveRBuffer;
}

void NetworkingModule::PumpClientServerUpdate(double time) {
  client->SendPackets();
  if (server) {
    server->SendPackets();
  }

  client->ReceivePackets();
  if (server) {
    server->ReceivePackets();
  }

  client->AdvanceTime(time);
  if (server) {
    server->AdvanceTime(time);
  }
}

void NetworkingModule::Connect(const char* serverAddress) {
  yojimbo::Address address(serverAddress);
  if (!address.IsValid()) {
    return;
  }

  client->InsecureConnect(privateKey, clientId, address);
}

void NetworkingModule::Disconnect() {
  if (client->IsConnected()) {
    client->Disconnect();
  }
}

void NetworkingModule::CreateServer(const char* address, int port) {
  serverAddress = yojimbo::Address("127.0.0.1", NetworkingModule::ServerPort);
  // TODO(Caleb): change out the memory allocation with our own custom allocator
  server = new yojimbo::Server(yojimbo::GetDefaultAllocator(), privateKey, serverAddress,
                               networkConfig, NetworkingModule::NetworkAdapter, clock.GetElapsedTime());
  server->Start(MaxClients);

  if (!server->IsRunning()) {
    return;
  }
}

void NetworkingModule::CloseServer() {
  if (server->IsRunning()) {
    server->Stop();
  }
}
}  // namespace Isetta
