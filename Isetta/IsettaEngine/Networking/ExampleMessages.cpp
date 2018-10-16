/*
 * Copyright (c) 2018 Isetta
 */

#include "ExampleMessages.h"
#include "Networking/NetworkIdentity.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {

namespace NetworkingExample {

int despawnCounter = 1;
std::vector<Entity*> spawnedEntities;

U16 exampleClientHandleId;
U16 exampleServerHandleId;
U16 exampleClientStringId;
U16 exampleServerStringId;

U16 exampleClientSpawn;
U16 exampleServerSpawn;
U16 exampleClientDespawn;
U16 exampleServerDespawn;

void InitExampleMessages() {
  RPC_MESSAGE_INIT(HandleMessage, "HNDL");
  RPC_MESSAGE_INIT(StringMessage, "STRN");
  RPC_MESSAGE_INIT(SpawnExample, "SPWN");
  RPC_MESSAGE_INIT(DespawnExample, "DSPN");
}

void RegisterExampleMessageFunctions() {
  exampleClientHandleId = NetworkManager::RegisterClientCallback(
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

  exampleServerHandleId = NetworkManager::RegisterServerCallback(
      "HNDL", [](int clientIdx, yojimbo::Message* message) {
        HandleMessage* handleMessage =
            reinterpret_cast<HandleMessage*>(message);
        LOG(Debug::Channel::Networking, "Client %d sends handle #%d", clientIdx,
            handleMessage->handle);

        NetworkManager::SendAllMessageFromServer("HNDL", handleMessage);
      });

  exampleClientStringId = NetworkManager::RegisterClientCallback(
      "STRN", [](yojimbo::Message* message) {
        StringMessage* stringMessage = static_cast<StringMessage*>(message);
        LOG(Debug::Channel::Networking, "Server says: %s",
            stringMessage->string.c_str());
      });

  exampleServerStringId = NetworkManager::RegisterServerCallback(
      "STRN", [](int clientIdx, yojimbo::Message* message) {
        StringMessage* stringMessage =
            reinterpret_cast<StringMessage*>(message);
        LOG(Debug::Channel::Networking, "Client %d says: %s", clientIdx,
            stringMessage->string.c_str());
      });

  exampleClientSpawn = NetworkManager::RegisterClientCallback(
      "SPWN", [](yojimbo::Message* message) {
        if (NetworkManager::ServerIsRunning()) {
          return;
        }

        SpawnExample* spawnMessage = reinterpret_cast<SpawnExample*>(message);
        const Entity* entity =
            NetworkManager::GetNetworkEntity(spawnMessage->netId);
        if (!entity) {
          Entity* e = LevelManager::Instance().currentLevel->AddEntity(
              Util::StrFormat("NetworkEntity%d", spawnMessage->netId));
          e->AddComponent<NetworkIdentity>(true, e, spawnMessage->netId);
          spawnedEntities.push_back(e);
        }
      });

  exampleServerSpawn = NetworkManager::RegisterServerCallback(
      "SPWN", [](int clientIdx, yojimbo::Message* message) {
        static int count = 1;
        SpawnExample* spawnMessage = reinterpret_cast<SpawnExample*>(message);

        if (!spawnMessage->netId) {
          Entity* e = LevelManager::Instance().currentLevel->AddEntity(
              Util::StrFormat("NetworkEntity%d", count++));
          NetworkIdentity* netIdentity =
              e->AddComponent<NetworkIdentity>(true, e);
          spawnedEntities.push_back(e);
          spawnMessage->netId = netIdentity->id;
        }

        NetworkManager::SendAllMessageFromServer("SPWN", spawnMessage);
      });

  exampleClientDespawn = NetworkManager::RegisterClientCallback(
      "DSPN", [](yojimbo::Message* message) {
        if (NetworkManager::ServerIsRunning()) {
          return;
        }

        DespawnExample* despawnMessage =
            reinterpret_cast<DespawnExample*>(message);
        Entity* entity =
            NetworkManager::GetNetworkEntity(despawnMessage->netId);
        if (!entity) {
          return;
        }
        NetworkManager::RemoveNetworkId(
            entity->GetComponent<NetworkIdentity>());
        entity->SetActive(false);
        entity->SetTransform(Math::Vector3(10000.0, 10000.0, 10000.0));
        //Entity::Destroy(entity);
      });

  exampleServerDespawn = NetworkManager::RegisterServerCallback(
      "DSPN", [](int clientIdx, yojimbo::Message* message) {
        DespawnExample* despawnMessage =
            reinterpret_cast<DespawnExample*>(message);

        if (!despawnMessage->netId) {
          return;
        }

        NetworkManager::SendAllMessageFromServer("DSPN", despawnMessage);

        Entity* entity =
            NetworkManager::GetNetworkEntity(despawnMessage->netId);
        if (!entity) {
          return;
        }

        NetworkManager::RemoveNetworkId(
            entity->GetComponent<NetworkIdentity>());
        entity->SetActive(false);
        entity->SetTransform(Math::Vector3(10000.0, 10000.0, 10000.0));
        //Entity::Destroy(entity);
      });
}

void DeregisterExampleMessageFunctions() {
  NetworkManager::UnregisterClientCallback("HNDL", exampleClientHandleId);
  NetworkManager::UnregisterServerCallback("HNDL", exampleServerHandleId);
  NetworkManager::UnregisterClientCallback("STRN", exampleClientStringId);
  NetworkManager::UnregisterServerCallback("STRN", exampleServerStringId);
  NetworkManager::UnregisterClientCallback("SPWN", exampleClientSpawn);
  NetworkManager::UnregisterClientCallback("SPWN", exampleServerSpawn);
  NetworkManager::UnregisterClientCallback("DSPN", exampleClientDespawn);
  NetworkManager::UnregisterClientCallback("DSPN", exampleServerDespawn);

  while (spawnedEntities.size() > 0) {
    Entity::Destroy(spawnedEntities[spawnedEntities.size() - 1]);
    spawnedEntities.pop_back();
  }
}
}  // namespace NetworkingExample
}  // namespace Isetta