/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkingModule.h"
#include <stdexcept>
#include <string>
#include "Core/Debug/Logger.h"

namespace Isetta {
CustomAdapter NetworkingModule::NetworkAdapter;
void NetworkingModule::StartUp() {
  if (!InitializeYojimbo()) {
    throw std::exception(
        "NetworkingModule::StartUp() Could not initialize yojimbo.");
  }

  // TODO(Caleb): Add CVar inputs for the config options
  networkConfig.numChannels = 1;
  networkConfig.channel[0].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;

  clientAddress = yojimbo::Address("0.0.0.0", NetworkingModule::ClientPort);
  // TODO(Caleb): change out the memory allocation with our own custom allocator
  client = new yojimbo::Client(yojimbo::GetDefaultAllocator(), clientAddress,
                               networkConfig, NetworkingModule::NetworkAdapter,
                               clock.GetElapsedTime());

  privateKey = new uint8_t[KeyBytes];
  // TODO(Caleb): Need to do something more insightful with the private key than
  // all 0s
  memset(privateKey, 0, KeyBytes);

  yojimbo::random_bytes(reinterpret_cast<uint8_t*>(&clientId), 8);

  clientSendBuffer = RingBuffer<yojimbo::Message*>(ClientQueueSize);
}

void NetworkingModule::Update(float deltaTime) {
  clock.UpdateTime();

  // Check for new connections
  for (int i = 0; i < NumIterations; i++) {
    PumpClientServerUpdate(clock.GetElapsedTime());

    if (client->ConnectionFailed() || (server && !server->IsRunning())) {
      break;
    }

    if (!server && !client->IsConnecting() && client->IsConnected()) {
      break;
    }
  }

  // Send out our messages
  SendClientToServerMessages();
  if (server) {
    for (int i = 0; i < MaxClients; i++) {
      SendServerToClientMessages(i);
    }
  }

  // Receive and process the messages from the other side
  for (int i = 0; i < NumIterations; i++) {
    PumpClientServerUpdate(clock.GetElapsedTime());

    if (client->IsConnected()) {
      ProcessServerToClientMessages();
    }

    if (server) {
      for (int i = 0; i < MaxClients; i++) {
        ProcessClientToServerMessages(i);
      }
    }
  }
}

void NetworkingModule::ShutDown() {
  // TODO(Caleb): Change the mem dealloc with our new manager
  delete client;
  if (server) {
    delete server;
  }
  delete privateKey;
}

// NOTE: Deletes the oldest message in the queue if the queue is
// overflowing--it would be nice to return it, but then we wouldn't
// be able to guarantee the memory is reused
void NetworkingModule::AddClientToServerMessage(yojimbo::Message* message) {
  if (clientSendBuffer.IsFull()) {
    client->ReleaseMessage(
        clientSendBuffer.Get());  // This may be horribly wrong
  }
  clientSendBuffer.Put(message);
}

// NOTE: Deletes the oldest message in the queue if the queue is
// overflowing--it would be nice to return it, but then we wouldn't
// be able to guarantee the memory is reused
void NetworkingModule::AddServerToClientMessage(int clientIdx,
                                                yojimbo::Message* message) {
  if (serverSendBufferArray[clientIdx].IsFull()) {
    server->ReleaseMessage(
        clientIdx,
        serverSendBufferArray[clientIdx].Get());  // This may be horribly wrong
  }
  serverSendBufferArray[clientIdx].Put(message);
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

void NetworkingModule::SendClientToServerMessages() {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  while (!clientSendBuffer.IsEmpty()) {
    if (!client->CanSendMessage(channelIdx)) {
      break;
    }

    yojimbo::Message* message = clientSendBuffer.Get();
    client->SendMessage(channelIdx, message);
  }
}

void NetworkingModule::SendServerToClientMessages(int clientIdx) {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  while (!serverSendBufferArray[clientIdx].IsEmpty()) {
    if (!server->CanSendMessage(clientIdx, channelIdx)) {
      break;
    }

    yojimbo::Message* message = serverSendBufferArray[clientIdx].Get();
    server->SendMessage(clientIdx, channelIdx, message);
  }
}

void NetworkingModule::ProcessClientToServerMessages(int clientIdx) {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  for (;;) {
    yojimbo::Message* message = server->ReceiveMessage(clientIdx, channelIdx);

    if (!message) {
      break;
    }

    switch (message->GetType()) {
      case HANDLE_MESSAGE: {
        HandleMessage* handleMessage =
            reinterpret_cast<HandleMessage*>(message);
        LOG(Debug::Channel::Networking, "Client %d sends handle #%d", clientIdx,
            handleMessage->handle);
      } break;
      case STRING_MESSAGE: {
        StringMessage* stringMessage =
            reinterpret_cast<StringMessage*>(message);
        LOG(Debug::Channel::Networking, "Client %d says: %s", clientIdx,
            stringMessage->string.c_str());
      } break;
    }

    server->ReleaseMessage(clientIdx, message);
  }
}

void NetworkingModule::ProcessServerToClientMessages() {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  for (;;) {
    yojimbo::Message* message = client->ReceiveMessage(channelIdx);

    if (!message) {
      break;
    }

    switch (message->GetType()) {
      case HANDLE_MESSAGE: {
        HandleMessage* handleMessage =
            reinterpret_cast<HandleMessage*>(message);
        LOG(Debug::Channel::Networking, "Server sends handle #%d",
            handleMessage->handle);
      } break;
      case STRING_MESSAGE: {
        StringMessage* stringMessage =
            reinterpret_cast<StringMessage*>(message);
        LOG(Debug::Channel::Networking, "Server says: %s",
            stringMessage->string.c_str());
      } break;
    }

    client->ReleaseMessage(message);
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
  if (server) {
    throw std::exception(
        "NetworkingModule::CreateServer() Cannot create a server while one is "
        "already running.");
  }
  serverSendBufferArray = new RingBuffer<yojimbo::Message*>[MaxClients];
  for (int i = 0; i < MaxClients; i++) {
    serverSendBufferArray[i] =
        RingBuffer<yojimbo::Message*>(ServerQueueSizePerClient);
  }

  serverAddress = yojimbo::Address("127.0.0.1", NetworkingModule::ServerPort);
  // TODO(Caleb): change out the memory allocation with our own custom allocator
  server = new yojimbo::Server(
      yojimbo::GetDefaultAllocator(), privateKey, serverAddress, networkConfig,
      NetworkingModule::NetworkAdapter, clock.GetElapsedTime());
  server->Start(MaxClients);

  if (!server->IsRunning()) {
    return;
  }
}

void NetworkingModule::CloseServer() {
  if (!server->IsRunning()) {
    throw std::exception(
        "NetworkingModule::CloseServer() Cannot close the server if it is not "
        "running.");
  }
  server->Stop();
  delete server;
  delete[] serverSendBufferArray;
}
}  // namespace Isetta
