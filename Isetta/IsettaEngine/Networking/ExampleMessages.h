/*
 * Copyright (c) 2018 Isetta
 */

#pragma once

#include <string>
#include "Audio/AudioSource.h"
#include "Core/Debug/Logger.h"
#include "Networking/Messages.h"
#include "Networking/NetworkManager.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

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

SpawnExample() {}

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);
  serialize_float(stream, a);
  serialize_float(stream, b);
  serialize_float(stream, c);
}

void Copy(const yojimbo::Message* otherMessage) override {
  const SpawnExample* message =
      reinterpret_cast<const SpawnExample*>(otherMessage);
  netId = message->netId;
  a = message->a;
  b = message->b;
  c = message->c;
}

U32 netId;
float a, b, c;

RPC_MESSAGE_FINISH

// Despawn
RPC_MESSAGE_DEFINE(DespawnExample)

DespawnExample() {}

template <typename Stream>
bool Serialize(Stream* stream) {
  serialize_int(stream, netId, 0, 256);
}

void Copy(const yojimbo::Message* otherMessage) override {
  const DespawnExample* message =
      reinterpret_cast<const DespawnExample*>(otherMessage);
  netId = message->netId;
}

U32 netId;

RPC_MESSAGE_FINISH

inline void InitExampleMessages() {
  RPC_MESSAGE_INIT(HandleMessage, "HNDL");
  RPC_MESSAGE_INIT(StringMessage, "STRN");
  RPC_MESSAGE_INIT(SpawnExample, "SPWN");
  RPC_MESSAGE_INIT(DespawnExample, "DSPN");
}

namespace NetworkingExample {
U16 exampleClientHandleId = 0;
U16 exampleServerHandleId = 0;
U16 exampleClientStringId = 0;
U16 exampleServerStringId = 0;

U16 exampleClientSpawn = 0;
U16 exampleServerSpawn = 0;
U16 exampleClientDespawn = 0;
U16 exampleServerDespawn = 0;

std::vector<Entity*> spawnedEntities;

inline void RegisterExampleMessageFunctions() {
  exampleClientHandleId = NetworkRegistry::RegisterClientCallback(
      "HNDL", [](yojimbo::Message* message) {
        HandleMessage* handleMessage = static_cast<HandleMessage*>(message);
        LOG(Debug::Channel::Networking, "Server sends handle #%d",
            handleMessage->handle);
        if (handleMessage->handle == 0) {
          LOG(Debug::Channel::Networking,
              "Server says we should play the animation!");
        }
        if (handleMessage->handle == 1) {
          LOG(Debug::Channel::Networking,
              "Server says we should stop the animation!");
        }
        if (handleMessage->handle == 2) {
          AudioSource audio = AudioSource();
          audio.SetAudioClip("gunshot.aiff");
          audio.Play(false, 1.f);
        }
      });

  exampleServerHandleId = NetworkRegistry::RegisterServerCallback(
      "HNDL", [](int clientIdx, yojimbo::Message* message) {
        HandleMessage* handleMessage =
            reinterpret_cast<HandleMessage*>(message);
        LOG(Debug::Channel::Networking, "Client %d sends handle #%d", clientIdx,
            handleMessage->handle);

        NetworkManager::SendAllMessageFromServer("HNDL", handleMessage);
      });

  exampleClientStringId = NetworkRegistry::RegisterClientCallback(
      "STRN", [](yojimbo::Message* message) {
        StringMessage* stringMessage = static_cast<StringMessage*>(message);
        LOG(Debug::Channel::Networking, "Server says: %s",
            stringMessage->string.c_str());
      });

  exampleServerStringId = NetworkRegistry::RegisterServerCallback(
      "STRN", [](int clientIdx, yojimbo::Message* message) {
        StringMessage* stringMessage =
            reinterpret_cast<StringMessage*>(message);
        LOG(Debug::Channel::Networking, "Client %d says: %s", clientIdx,
            stringMessage->string.c_str());
      });

  exampleClientSpawn = NetworkRegistry::RegisterClientCallback(
    "SPWN", [](yojimbo::Message* message) {
        SpawnExample* spawnMessage = reinterpret_cast<SpawnExample*>(message);
        const Entity* entity =
            NetworkRegistry::GetNetworkEntity(spawnMessage->netId);
        if (!entity) {
          Entity* e = LevelManager::Instance().currentLevel->AddEntity(
              Util::StrFormat("NetworkEntity%d", spawnMessage->netId));
          e->AddComponent<NetworkIdentity>(spawnMessage->netId);
          spawnedEntities.push_back(e);
        }
  });

  exampleServerSpawn = NetworkRegistry::RegisterServerCallback(
      "SPWN", [](int clientIdx, yojimbo::Message* message) {
        SpawnExample* spawnMessage = reinterpret_cast<SpawnExample*>(message);
        const Entity* entity =
            NetworkRegistry::GetNetworkEntity(spawnMessage->netId);
        if (!entity) {
          Entity* e = LevelManager::Instance().currentLevel->AddEntity(
              Util::StrFormat("NetworkEntity%d", spawnMessage->netId));
          e->AddComponent<NetworkIdentity>(spawnMessage->netId);
          spawnedEntities.push_back(e);
        }

        NetworkManager::SendAllMessageFromServer("SPWN", spawnMessage);
  });

  exampleClientDespawn = NetworkRegistry::RegisterClientCallback(
    "DSPN", [](yojimbo::Message* message) {
        DespawnExample* despawnMessage =
            reinterpret_cast<DespawnExample*>(message);
        Entity* entity =
            NetworkRegistry::GetNetworkEntity(despawnMessage->netId);
        if (!entity) {
          return;
        }
        NetworkRegistry::RemoveNetworkId(despawnMessage->netId);
        entity->SetActive(false);
      });

  exampleServerDespawn = NetworkRegistry::RegisterServerCallback(
      "DSPN", [](int clientIdx, yojimbo::Message* message) {
        DespawnExample* despawnMessage =
            reinterpret_cast<DespawnExample*>(message);

        NetworkManager::SendAllMessageFromServer("DSPN", despawnMessage);

        Entity* entity =
            NetworkRegistry::GetNetworkEntity(despawnMessage->netId);
        if (!entity) {
          return;
        }
        NetworkRegistry::RemoveNetworkId(despawnMessage->netId);
        entity->SetActive(false);
      });
}

inline void DeregisterExampleMessageFunctions() {
  NetworkRegistry::UnregisterClientCallback("HNDL", exampleClientHandleId);
  NetworkRegistry::UnregisterServerCallback("HNDL", exampleServerHandleId);
  NetworkRegistry::UnregisterClientCallback("STRN", exampleClientStringId);
  NetworkRegistry::UnregisterServerCallback("STRN", exampleServerStringId);
  NetworkRegistry::UnregisterClientCallback("SPWN", exampleClientSpawn);
  NetworkRegistry::UnregisterClientCallback("SPWN", exampleServerSpawn);
  NetworkRegistry::UnregisterClientCallback("DSPN", exampleClientDespawn);
  NetworkRegistry::UnregisterClientCallback("DSPN", exampleServerDespawn);
}
}  // namespace NetworkingExample

}  // namespace Isetta
