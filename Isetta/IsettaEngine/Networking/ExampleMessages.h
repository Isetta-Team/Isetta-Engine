/*
 * Copyright (c) 2018 Isetta
 */

#pragma once

#include <string>
#include "Audio/AudioSource.h"
#include "Core/Debug/Logger.h"
#include "Networking/Messages.h"

namespace Isetta {

/**
 * @brief Code-generated struct to be used for sending integer values across the
 * network.
 *
 */
RPC_MESSAGE_DEFINE(HandleMessage)
HandleMessage() { handle = 0; }

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
int handle;

RPC_MESSAGE_FINISH

/**
 * @brief Code-generated struct to be used for sending string messages across
 * the network.
 *
 */
RPC_MESSAGE_DEFINE(StringMessage)

StringMessage() { string = ""; }

// TODO(Caleb): choose a more reasonable range for the int serialization
template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_string(stream, const_cast<char*>(string.c_str()), 512);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const StringMessage* message =
      reinterpret_cast<const StringMessage*>(otherMessage);
  string = message->string;
}

public:
std::string string;

RPC_MESSAGE_FINISH

// Spawn
RPC_MESSAGE_DEFINE(SpawnExample)

SpawnExample() { netId = a = b = c = 0; }

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);
  serialize_float(stream, a);
  serialize_float(stream, b);
  serialize_float(stream, c);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const SpawnExample* message =
      reinterpret_cast<const SpawnExample*>(otherMessage);
  netId = message->netId;
  a = message->a;
  b = message->b;
  c = message->c;
}

int netId;
float a, b, c;

RPC_MESSAGE_FINISH

// Despawn
RPC_MESSAGE_DEFINE(DespawnExample)

DespawnExample() { netId = 0; }

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);

  return true;
}

void Copy(const yojimbo::Message* otherMessage) override {
  const DespawnExample* message =
      reinterpret_cast<const DespawnExample*>(otherMessage);
  netId = message->netId;
}

int netId;

RPC_MESSAGE_FINISH

namespace NetworkingExample {
void InitExampleMessages();

extern U16 exampleClientHandleId;
extern U16 exampleServerHandleId;
extern U16 exampleClientStringId;
extern U16 exampleServerStringId;

extern U16 exampleClientSpawn;
extern U16 exampleServerSpawn;
extern U16 exampleClientDespawn;
extern U16 exampleServerDespawn;

extern int despawnCounter;
extern std::vector<Entity*> spawnedEntities;

void RegisterExampleMessageFunctions();
void DeregisterExampleMessageFunctions();
}  // namespace NetworkingExample

}  // namespace Isetta
