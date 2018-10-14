/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkingModule.h"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Graphics/AnimationComponent.h"
#include "Networking/NetworkManager.h"
#include "Networking/ExampleMessages.h"

// F Windows
#ifdef SendMessage
#undef SendMessage
#endif

namespace Isetta {

// Defining static variables
CustomAdapter NetworkingModule::NetworkAdapter;

int NetworkRegistry::count;
std::unordered_map<int, std::pair<U64, Func<yojimbo::Message*, void*>>>
    NetworkRegistry::factories;
std::unordered_map<int, Action<yojimbo::Client*, yojimbo::Message*>>
    NetworkRegistry::clientFuncs;
std::unordered_map<int, Action<int, yojimbo::Server*, yojimbo::Message*>>
    NetworkRegistry::serverFuncs;
std::unordered_map<const char*, int> NetworkRegistry::tags;

void NetworkingModule::StartUp() {
  NetworkManager::networkingModule = this;

  InitExampleMessages();

  if (!InitializeYojimbo()) {
    throw std::exception(
        "NetworkingModule::StartUp => Could not initialize yojimbo.");
  }
  srand((unsigned int)time(NULL));

  // TODO(Caleb): Figure out some more robust channel settings
  networkConfig.numChannels = 1;
  networkConfig.channel[0].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
  networkConfig.timeout = 20;

  privateKey = new (MemoryManager::AllocOnStack(
      sizeof(U8) * Config::Instance().networkConfig.keyBytes.GetVal()))
      U8[Config::Instance().networkConfig.keyBytes.GetVal()];
  // TODO(Caleb): Need to do something more insightful with the private key
  // than all 0s
  memset(privateKey, 0, Config::Instance().networkConfig.keyBytes.GetVal());

  clientId = 0;
  yojimbo::random_bytes(reinterpret_cast<U8*>(&clientId), 8);

  void* memPointer =
      MemoryManager::AllocOnStack(networkConfig.clientMemory + 1_MB);
  clientAllocator = new (MemoryManager::AllocOnStack(sizeof(NetworkAllocator)))
      NetworkAllocator(memPointer, (Size)networkConfig.clientMemory + 1_MB);

  if (Config::Instance().networkConfig.runServer.GetVal()) {
    Size serverMemorySize =
        (networkConfig.serverPerClientMemory +
         networkConfig.serverGlobalMemory) *
        (Config::Instance().networkConfig.maxClients.GetVal() + 1);

    memPointer = MemoryManager::AllocOnStack(serverMemorySize);
    serverAllocator =
        new (MemoryManager::AllocOnStack(sizeof(NetworkAllocator)))
            NetworkAllocator(memPointer, serverMemorySize);
  }

  client = new (MemoryManager::AllocOnStack(sizeof(yojimbo::Client)))
      yojimbo::Client(
          clientAllocator,
          yojimbo::Address(
              Config::Instance().networkConfig.defaultClientIP.GetVal().c_str(),
              Config::Instance().networkConfig.clientPort.GetVal()),
          networkConfig, NetworkAdapter, clock.GetElapsedTime());

  clientSendBuffer =
      MemoryManager::NewArrOnFreeList<RingBuffer<yojimbo::Message*>>(
          Config::Instance().networkConfig.clientQueueSize.GetVal());
}

void NetworkingModule::Update(float deltaTime) {
  clock.UpdateTime();

  // Check for new connections
  PumpClientServerUpdate(clock.GetElapsedTime());

  // Send out our messages
  SendClientToServerMessages();
  if (server) {
    for (int i = 0; i < Config::Instance().networkConfig.maxClients.GetVal();
         i++) {
      SendServerToClientMessages(i);
    }
  }

  // Receive and process the messages from the other side
  PumpClientServerUpdate(clock.GetElapsedTime());

  if (client->IsConnected()) {
    ProcessServerToClientMessages();
  }

  if (server) {
    for (int i = 0; i < Config::Instance().networkConfig.maxClients.GetVal();
         i++) {
      ProcessClientToServerMessages(i);
    }
  }
}

void NetworkingModule::ShutDown() {
  try {
    Disconnect();
  } catch (std::exception e) {
  }

  try {
    CloseServer();
  } catch (std::exception e) {
  }

  ShutdownYojimbo();
}

// NOTE: Deletes the oldest message in the queue if the queue is
// overflowing--it would be nice to return it, but then we wouldn't
// be able to guarantee the memory is reused
void NetworkingModule::AddClientToServerMessage(yojimbo::Message* message) {
  if (clientSendBuffer->IsFull()) {
    client->ReleaseMessage(
        clientSendBuffer->Get());  // This may be horribly wrong
  }
  clientSendBuffer->Put(message);
}

// NOTE: Deletes the oldest message in the queue if the queue is
// overflowing--it would be nice to return it, but then we wouldn't
// be able to guarantee the memory is reused
void NetworkingModule::AddServerToClientMessage(int clientIdx,
                                                yojimbo::Message* message) {
  if (serverSendBufferArray[clientIdx].IsFull()) {
    server->ReleaseMessage(
        clientIdx,
        serverSendBufferArray[clientIdx]
            .Get());  // TODO(Caleb): This may be horribly wrong; test that
                      // ReleaseMessage works from source
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
  while (!clientSendBuffer->IsEmpty()) {
    if (!client->CanSendMessage(channelIdx)) {
      break;
    }

    yojimbo::Message* message = clientSendBuffer->Get();
    client->SendMessage(channelIdx, message);  // bugged out
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

    NetworkRegistry::ServerFunc(message->GetType())(clientIdx, server, message);

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

    NetworkRegistry::ClientFunc(message->GetType())(client, message);

    client->ReleaseMessage(message);
  }
}

void NetworkingModule::Connect(const char* serverAddress, int serverPort,
                               Action<bool> callback) {
  yojimbo::Address address(serverAddress, serverPort);
  if (!address.IsValid()) {
    return;
  }

  client->InsecureConnect(privateKey, clientId, address, callback);
}

void NetworkingModule::Disconnect() {
  if (!client->IsConnecting()) {
    return;
  } else if (!client->IsConnected()) {
    throw std::exception(
        "NetworkingModule::Disconnect => Cannot disconnect the client if it is "
        "not already connected.");
  }
  client->Disconnect();
}

void NetworkingModule::CreateServer(const char* address, int port) {
  if (server && server->IsRunning()) {
    throw std::exception(
        "NetworkingModule::CreateServer => Cannot create a server while one is "
        "already running.");
  }
  serverSendBufferArray =
      MemoryManager::NewArrOnFreeList<RingBuffer<yojimbo::Message*>>(
          Config::Instance().networkConfig.maxClients.GetVal());
  for (int i = 0; i < Config::Instance().networkConfig.maxClients.GetVal();
       i++) {
    serverSendBufferArray[i] = RingBuffer<yojimbo::Message*>(
        Config::Instance().networkConfig.serverQueueSizePerClient.GetVal());
  }

  serverAddress = yojimbo::Address(address, port);
  server = MemoryManager::NewOnFreeList<yojimbo::Server>(
      serverAllocator, privateKey, serverAddress, networkConfig,
      &NetworkingModule::NetworkAdapter, clock.GetElapsedTime());
  server->Start(Config::Instance().networkConfig.maxClients.GetVal());

  if (!server->IsRunning()) {
    throw std::exception(
        "NetworkingModule::CreateServer => Unable to run server.");
  }
}

void NetworkingModule::CloseServer() {
  if (!server || !server->IsRunning()) {
    throw std::exception(
        "NetworkingModule::CloseServer() Cannot close the server if it is not "
        "running.");
  }
  server->Stop();
  MemoryManager::FreeOnFreeList(server);
  MemoryManager::FreeOnFreeList(serverSendBufferArray);
}
}  // namespace Isetta
