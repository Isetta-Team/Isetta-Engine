/*
 * Copyright (c) 2018 Isetta
 */

#include "Networking/NetworkingModule.h"

#include <list>
#include <utility>

#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/SystemInfo.h"
#include "Networking/BuiltinMessages.h"
#include "Networking/NetworkManager.h"
#include "brofiler/ProfilerCore/Brofiler.h"

// F Windows
#ifdef SendMessage
#undef SendMessage
#endif

namespace Isetta {

// Defining static variables
CustomAdapter NetworkingModule::NetworkAdapter;

void NetworkingModule::StartUp() {
  NetworkManager::Instance().networkingModule = this;
  if (!InitializeYojimbo()) {
    throw std::exception(
        "NetworkingModule::StartUp => Could not initialize yojimbo.");
  }
  srand(static_cast<unsigned int>(time(nullptr)));

  // TODO(Caleb): Figure out some more robust channel settings
  yojimboConfig.numChannels = 1;
  yojimboConfig.channel[0].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
  yojimboConfig.timeout = CONFIG_VAL(networkConfig.timeout);

  privateKey = new (MemoryManager::AllocOnStack(
      sizeof(U8) * CONFIG_VAL(networkConfig.keyBytes)))
      U8[CONFIG_VAL(networkConfig.keyBytes)];
  // TODO(Caleb): Need to do something more insightful with the private key
  // than all 0s
  memset(privateKey, 0, CONFIG_VAL(networkConfig.keyBytes));

  // Initialize client
  clientId = 0;
  yojimbo::random_bytes(reinterpret_cast<U8*>(&clientId), 8);

  clientAllocator = MemoryManager::NewOnStack<NetworkAllocator>(
      MemoryManager::AllocOnStack(yojimboConfig.clientMemory + 1_MB),
      static_cast<Size>(yojimboConfig.clientMemory) + 1_MB);

  client = new (MemoryManager::AllocOnStack(sizeof(yojimbo::Client)))
      yojimbo::Client(
          clientAllocator,
          yojimbo::Address(CONFIG_VAL(networkConfig.defaultClientIP).c_str(),
                           CONFIG_VAL(networkConfig.clientPort)),
          yojimboConfig, NetworkAdapter, clock.GetElapsedTime());

  clientSendBuffer =
      MemoryManager::NewArrOnFreeList<RingBuffer<yojimbo::Message*>>(
          CONFIG_VAL(networkConfig.clientQueueSize));
}

void NetworkingModule::Update(float deltaTime) {
  BROFILER_CATEGORY("Network Update", Profiler::Color::Orange);

  clock.UpdateTime();

  // Check for new connections
  PumpClientServerUpdate(clock.GetElapsedTime());

  // Send out our messages
  SendClientToServerMessages();
  int maxClients = CONFIG_VAL(networkConfig.maxClients);
  if (server) {
    for (int i = 0; i < maxClients; ++i) {
      SendServerToClientMessages(i);
    }
  }

  // Receive and process the messages from the other side
  PumpClientServerUpdate(clock.GetElapsedTime());

  if (client->IsConnected()) {
    ProcessServerToClientMessages();
  }

  if (server) {
    for (int i = 0; i < maxClients; ++i) {
      ProcessClientToServerMessages(i);
    }
  }

  // State monitor
  if (wasClientRunningLastFrame && !IsClientRunning()) {
    // client dropped out
    onDisconnectedFromServer.Invoke();
  }
  wasClientRunningLastFrame = IsClientRunning();

  if (IsServerRunning()) {
    for (int i = 0; i < maxClients; ++i) {
      if (wasClientConnectedLastFrame[i] && !IsClientConnected(i)) {
        // client just disconnected
        onClientDisconnected.Invoke(clientInfos[i]);
      }
      wasClientConnectedLastFrame[i] = IsClientConnected(i);
    }
  }
}

void NetworkingModule::ShutDown() {
  try {
    Disconnect();
  } catch (std::exception& e) {
  }

  try {
    CloseServer();
  } catch (std::exception& e) {
  }

  ShutdownYojimbo();
  client->~Client();
  MemoryManager::DeleteArrOnFreeList<RingBuffer<yojimbo::Message*>>(
      CONFIG_VAL(networkConfig.clientQueueSize), clientSendBuffer);
  clientAllocator->~NetworkAllocator();
}

// NOTE: Deletes the oldest message in the queue if the queue is
// overflowing--it would be nice to return it, but then we wouldn't
// be able to guarantee the memory is reused
void NetworkingModule::AddClientToServerMessage(
    yojimbo::Message* message) const {
  if (!IsClientRunning()) {
    LOG_ERROR(Debug::Channel::Networking,
              "Cannot send message from client cause client is not running");
    return;
  }
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
  if (!IsServerRunning()) {
    LOG_ERROR(Debug::Channel::Networking,
              "Cannot send message from server cause server is not running");
    return;
  }
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
  PROFILE
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

void NetworkingModule::SendClientToServerMessages() const {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  while (!clientSendBuffer->IsEmpty()) {
    if (!client->CanSendMessage(channelIdx)) {
      break;
    }

    yojimbo::Message* message = clientSendBuffer->Get();
    client->SendMessage(channelIdx, message);  // bugged out
  }
}

void NetworkingModule::SendServerToClientMessages(int clientIdx) const {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  while (!serverSendBufferArray[clientIdx].IsEmpty()) {
    if (!server->CanSendMessage(clientIdx, channelIdx)) {
      break;
    }

    yojimbo::Message* message = serverSendBufferArray[clientIdx].Get();
    server->SendMessage(clientIdx, channelIdx, message);
  }
}

void NetworkingModule::ProcessClientToServerMessages(int clientIdx) const {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  for (;;) {
    yojimbo::Message* message = server->ReceiveMessage(clientIdx, channelIdx);

    if (!message) {
      break;
    }

    auto serverFunctions =
        NetworkManager::Instance().GetServerFunctions(message->GetType());
    for (const auto& function : serverFunctions) {
      function.second(clientIdx, message);
    }

    server->ReleaseMessage(clientIdx, message);
  }
}

void NetworkingModule::ProcessServerToClientMessages() const {
  const int channelIdx = 0;  // TODO(Caleb): Upgrade the channel indexing
  for (;;) {
    yojimbo::Message* message = client->ReceiveMessage(channelIdx);

    if (!message) {
      break;
    }

    auto clientFunctions =
        NetworkManager::Instance().GetClientFunctions(message->GetType());
    for (const auto& function : clientFunctions) {
      function.second(message);
    }

    client->ReleaseMessage(message);
  }
}

void NetworkingModule::Connect(const char* serverAddress, int serverPort,
                               Action<NetworkManager::ClientState> callback) {
  if (IsClientRunning()) {
    LOG_ERROR(Debug::Channel::Networking,
              "NetworkingModule::Connect => Already running as client. Cannot "
              "StartClient again");
    return;
  }

  yojimbo::Address address(serverAddress, serverPort);
  if (!address.IsValid()) {
    LOG_ERROR(
        Debug::Channel::Networking,
        "NetworkingModule::Connect => IP Address for StartClient is invalid");
    return;
  }

  Action<int> internalCallback = [=](int state) {
    if (callback != nullptr) {
      callback(static_cast<NetworkManager::ClientState>(state));
    }
    if (state == static_cast<int>(NetworkManager::ClientState::Connected)) {
      NetworkManager::Instance().SendMessageFromClient<ClientConnectedMessage>(
          [](ClientConnectedMessage* message) {
            strcpy_s(message->ip, SystemInfo::GetIpAddressWithPrefix(
                                 CONFIG_VAL(networkConfig.ipPrefix))
                                 .c_str());
            strcpy_s(message->machineName, SystemInfo::GetMachineName().c_str());
          });

      this->onConnectedToServer.Invoke();
      this->wasClientRunningLastFrame = true;
    } else if (state < 0) {
      LOG_ERROR(Debug::Channel::Networking,
                "NetworkingModule::Connect => Failed to connected to %s as a "
                "client, Client State: %d",
                serverAddress, state);
    }
  };
  client->InsecureConnect(privateKey, clientId, address, internalCallback);

  // Register callbacks
  loadLevelCallbackHandle =
      NetworkManager::Instance().RegisterClientCallback<LoadLevelMessage>(
          [](yojimbo::Message* inMessage) {
            auto* loadLevelMessage =
                reinterpret_cast<LoadLevelMessage*>(inMessage);
            LevelManager::Instance().LoadLevel(loadLevelMessage->levelName);
          });
}

void NetworkingModule::Disconnect() const {
  if (client->IsConnected()) {
    client->Disconnect();
    // Unregister callbacks
    NetworkManager::Instance().UnregisterClientCallback<LoadLevelMessage>(
        loadLevelCallbackHandle);

  } else if (!client->IsConnecting()) {
    return;
  } else {
    throw std::exception(
        "NetworkingModule::Disconnect => Cannot disconnect the client if it is "
        "not already connected.");
  }
}

void NetworkingModule::CreateServer(const char* address, int port) {
  if (IsServerRunning()) {
    LOG_ERROR(
        Debug::Channel::Networking,
        "NetworkingModule::CreateServer => Cannot create a server while one is "
        "already running.");
    return;
  }

  // Create the server allocator
  Size serverMemorySize =
    (yojimboConfig.serverPerClientMemory + yojimboConfig.serverGlobalMemory) *
    (CONFIG_VAL(networkConfig.maxClients) + 1);

  serverAllocator = new (MemoryManager::AllocOnStack(sizeof(NetworkAllocator)))
      NetworkAllocator(MemoryManager::AllocOnStack(serverMemorySize),
                       serverMemorySize);

  // Create the buffers for the server messages
  int maxClients = CONFIG_VAL(networkConfig.maxClients);
  serverSendBufferArray =
      MemoryManager::NewArrOnFreeList<RingBuffer<yojimbo::Message*>>(
          maxClients);
  for (int i = 0; i < CONFIG_VAL(networkConfig.maxClients); ++i) {
    serverSendBufferArray[i] = RingBuffer<yojimbo::Message*>(
        CONFIG_VAL(networkConfig.serverQueueSizePerClient));
  }

  serverAddress = yojimbo::Address(address, port);
  server = MemoryManager::NewOnFreeList<yojimbo::Server>(
      serverAllocator, privateKey, serverAddress, yojimboConfig,
      &NetworkingModule::NetworkAdapter, clock.GetElapsedTime());
  server->Start(maxClients);

  if (!server->IsRunning()) {
    throw std::exception(
        "NetworkingModule::CreateServer => Unable to run server.");
  }

  wasClientConnectedLastFrame = MemoryManager::NewArrOnStack<bool>(maxClients);
  for (int i = 0; i < maxClients; ++i) {
    wasClientConnectedLastFrame[i] = false;
  }

  clientInfos = MemoryManager::NewArrOnFreeList<ClientInfo>(maxClients);

  clientConnectedCallbackHandle =
      NetworkManager::Instance().RegisterServerCallback<ClientConnectedMessage>(
          [this](const int clientIndex, yojimbo::Message* inMessage) {
            const ClientConnectedMessage* message =
                reinterpret_cast<const ClientConnectedMessage*>(inMessage);
            ClientInfo info;
            info.ip = message->ip;
            info.machineName = message->machineName;
            info.clientIndex = clientIndex;
            this->clientInfos[clientIndex] = info;
            this->onClientConnected.Invoke(info);
          });
}

void NetworkingModule::CloseServer() {
  if (!IsServerRunning()) {
    throw std::exception(
        "NetworkingModule::CloseServer() Cannot close the server if it is not "
        "running.");
  }

  MemoryManager::DeleteArrOnFreeList<ClientInfo>(
      CONFIG_VAL(networkConfig.maxClients), clientInfos);
  clientInfos = nullptr;
  server->Stop();
  MemoryManager::DeleteOnFreeList<yojimbo::Server>(server);
  server = nullptr;
  MemoryManager::DeleteArrOnFreeList<RingBuffer<yojimbo::Message*>>(
      CONFIG_VAL(networkConfig.maxClients), serverSendBufferArray);
  serverAllocator->~NetworkAllocator();

  NetworkManager::Instance().UnregisterClientCallback<ClientConnectedMessage>(
      clientConnectedCallbackHandle);
}

bool NetworkingModule::IsClient() const {
  return client->IsConnected() && !server || (server && !server->IsRunning());
}

bool NetworkingModule::IsHost() const {
  return client->IsConnected() && server && server->IsRunning();
}

bool NetworkingModule::IsServer() const {
  return !client->IsConnected() && server && server->IsRunning();
}

bool NetworkingModule::IsClientRunning() const {
  return client && client->IsConnected();
}

bool NetworkingModule::IsServerRunning() const {
  return server && server->IsRunning();
}

bool NetworkingModule::IsClientConnected(const int clientIndex) const {
  return IsServerRunning() && server->IsClientConnected(clientIndex);
}
}  // namespace Isetta
