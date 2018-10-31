/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

#include <string>
#include "Audio/AudioSource.h"
#include "Core/Debug/Logger.h"
#include "Networking/Messages.h"

namespace Isetta {
CREATE_LEVEL(NetworkLevel)
void LoadLevel() override;
};

/**
 * @brief Code-generated struct to be used for sending integer values across the
 * network.
 *
 */
RPC_MESSAGE_DEFINE(HandleMessage)
// TODO(Caleb): choose a more reasonable range for the int serialization
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, handle, 0, 64);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const HandleMessage* message =
      reinterpret_cast<const HandleMessage*>(otherMessage);
  handle = message->handle;
}

public:
int handle = 0;

RPC_MESSAGE_FINISH

// Spawn
RPC_MESSAGE_DEFINE(SpawnMessage)

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);
  serialize_int(stream, clientAuthorityId, 0,
                NetworkManager::Instance().GetMaxClients());
  serialize_float(stream, a);
  serialize_float(stream, b);
  serialize_float(stream, c);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const SpawnMessage* message =
      reinterpret_cast<const SpawnMessage*>(otherMessage);
  netId = message->netId;
  clientAuthorityId = message->clientAuthorityId;
  a = message->a;
  b = message->b;
  c = message->c;
}

int netId = 0, clientAuthorityId = 0;
float a = 0, b = 0, c = 0;

RPC_MESSAGE_FINISH

// Despawn
RPC_MESSAGE_DEFINE(DespawnMessage)

DespawnMessage() { netId = 0; }

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const DespawnMessage* message =
      reinterpret_cast<const DespawnMessage*>(otherMessage);
  netId = message->netId;
}

int netId;

RPC_MESSAGE_FINISH
}  // namespace Isetta