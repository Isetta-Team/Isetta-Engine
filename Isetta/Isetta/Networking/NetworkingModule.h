/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "yojimbo/yojimbo.h"
#include "Core/Time/Clock.h"

namespace Isetta {
struct CustomMessage : public yojimbo::Message {
  int handle;

  CustomMessage() { handle = 0; }

  // TODO(Caleb): choose a more reasonable range for the int serialization
  template <typename Stream>
  bool Serialize(Stream& stream) {
    serialize_int(stream, handle, 0, 10);

    return true;
  }

  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

enum CustomMessageType { MESSAGE, NUM_MESSAGE_TYPES };

YOJIMBO_MESSAGE_FACTORY_START(CustomMessageFactory, NUM_MESSAGE_TYPES);
  YOJIMBO_DECLARE_MESSAGE_TYPE(MESSAGE, CustomMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

class CustomAdapter : public yojimbo::Adapter {
 public:
  // TODO(Caleb): Change the CreateAllocator function to use our mem alloc
  // instead of TLSF

  // TODO(Caleb): something about the Linter with a const ref
  yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator* allocator) {
    return YOJIMBO_NEW(*allocator, CustomMessageFactory, *allocator);
  }
};

class NetworkingModule {
 private:
  static CustomAdapter NetworkAdapter;

  // TODO(Caleb): Look into making the ports user-definable
  const uint16_t ClientPort = 30000;
  const uint16_t ServerPort = 40000;
  const int KeyBytes = 32;
  const int MaxClients = 64;
  const int NumIterations = 1000;
  const int ClientQueueSize = 256;
  const int ServerQueueSize = 1024;

  Clock clock;

  yojimbo::Address clientAddress;
  yojimbo::Client* client;
  yojimbo::Address serverAddress;
  yojimbo::Server* server;
  yojimbo::ClientServerConfig networkConfig;

  uint8_t* privateKey;
  uint64_t clientId;

  CustomMessage* clientSendRBuffer;
  CustomMessage* clientReceiveRBuffer;
  CustomMessage* serverSendRBuffer;
  CustomMessage* serverReceiveRBuffer;

  // Constructors

  NetworkingModule() = default;
  ~NetworkingModule() = default;

  // Module functions

  void StartUp();
  void Update();
  void ShutDown();

  // Functions

  void PumpClientServerUpdate(double time);
  void SendClientToServerMessages();
  void SendServerToClientMessages();
  void ProcessClientToServerMessages();
  void ProcessServerToClientMessages();

  void Connect(const char* serverAddress);
  void Disconnect();

  void CreateServer(const char* address, int port);
  void CloseServer();

  // Other

  friend class ModuleManager;
};
}  // namespace Isetta
