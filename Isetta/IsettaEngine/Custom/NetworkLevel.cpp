/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/NetworkLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Core/Math/Math.h"
#include "Custom/KeyTransform.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Input/Input.h"
#include "Networking/NetworkId.h"
#include "Networking/NetworkTransform.h"
#include "Scene/Entity.h"
#include "EscapeExit.h"

using namespace Isetta;

int spawnCounter = 0;
int despawnCounter = 0;
std::list<Entity*> spawnedEntities;

U16 exampleClientHandleId;
U16 exampleServerHandleId;
U16 exampleClientStringId;
U16 exampleServerStringId;

U16 exampleClientSpawn;
U16 exampleServerSpawn;
U16 exampleClientDespawn;
U16 exampleServerDespawn;

void RegisterExampleMessageFunctions() {
  exampleClientHandleId =
      NetworkManager::Instance().RegisterClientCallback<HandleMessage>(
          [](yojimbo::Message* message) {
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
              AudioSource audio = AudioSource("Sound\\gunshot.aiff");
              // AudioSource(true, false, 1.f, "Sound\\gunshot.aiff");
              audio.Play();
            }
          });

  exampleServerHandleId =
      NetworkManager::Instance().RegisterServerCallback<HandleMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            HandleMessage* handleMessage =
                reinterpret_cast<HandleMessage*>(message);
            LOG(Debug::Channel::Networking, "Client %d sends handle #%d",
                clientIdx, handleMessage->handle);

            NetworkManager::Instance().SendAllMessageFromServer<HandleMessage>(
                handleMessage);
          });

  exampleClientSpawn =
      NetworkManager::Instance().RegisterClientCallback<SpawnMessage>(
          [](yojimbo::Message* message) {
            if (NetworkManager::Instance().ServerIsRunning()) {
              return;
            }

            SpawnMessage* spawnMessage =
                reinterpret_cast<SpawnMessage*>(message);

            Entity* entity = NetworkManager::Instance().GetNetworkEntity(
                spawnMessage->netId);
            if (!entity) {
              Entity* e = Entity::CreateEntity(
                  Util::StrFormat("NetworkEntity (%d)", spawnMessage->netId));
              NetworkId* netId =
                  e->AddComponent<NetworkId>(spawnMessage->netId);
              netId->clientAuthorityId = spawnMessage->clientAuthorityId;
              spawnedEntities.push_back(e);

              // Zombie
              e->transform->SetLocalScale(Math::Vector3::one * .01);
              MeshComponent* mesh = e->AddComponent<MeshComponent, true>(
                  "Zombie/Zombie.scene.xml");
              if (netId->HasClientAuthority()) {
                e->AddComponent<KeyTransform>();
              }
              e->AddComponent<NetworkTransform>();

              if (spawnMessage->a) {
                Entity* parent = NetworkManager::Instance().GetNetworkEntity(
                    spawnMessage->a);

                e->transform->SetParent(parent->transform);
                e->transform->SetLocalScale(Math::Vector3::one);
              }
            }
          });

  exampleServerSpawn =
      NetworkManager::Instance().RegisterServerCallback<SpawnMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            static int count = 1;
            SpawnMessage* spawnMessage =
                reinterpret_cast<SpawnMessage*>(message);

            if (!spawnMessage->netId) {
              Entity* e = Entity::CreateEntity(
                  Util::StrFormat("NetworkEntity%d", count++));
              NetworkId* netId = e->AddComponent<NetworkId>();
              netId->clientAuthorityId = clientIdx;
              spawnedEntities.push_back(e);
              spawnMessage->netId = netId->id;
              spawnMessage->clientAuthorityId = clientIdx;

              // Zombie
              e->transform->SetLocalScale(Math::Vector3::one * .01);
              MeshComponent* mesh = e->AddComponent<MeshComponent, true>(
                  "Zombie/Zombie.scene.xml");
              if (netId->HasClientAuthority()) {
                e->AddComponent<KeyTransform>();
              }
              e->AddComponent<NetworkTransform>();

              if (spawnMessage->a) {
                Entity* parent = NetworkManager::Instance().GetNetworkEntity(
                    spawnMessage->a);

                e->transform->SetParent(parent->transform);
                e->transform->SetLocalScale(Math::Vector3::one);
              }
            }

            NetworkManager::Instance().SendAllMessageFromServer<SpawnMessage>(
                spawnMessage);
          });

  exampleClientDespawn =
      NetworkManager::Instance().RegisterClientCallback<DespawnMessage>(
          [](yojimbo::Message* message) {
            if (NetworkManager::Instance().ServerIsRunning()) {
              return;
            }

            DespawnMessage* despawnMessage =
                reinterpret_cast<DespawnMessage*>(message);
            Entity* entity = NetworkManager::Instance().GetNetworkEntity(
                despawnMessage->netId);
            if (!entity) {
              return;
            }

            spawnedEntities.remove(entity);
            Entity::Destroy(entity);
          });

  exampleServerDespawn =
      NetworkManager::Instance().RegisterServerCallback<DespawnMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            DespawnMessage* despawnMessage =
                reinterpret_cast<DespawnMessage*>(message);

            if (!despawnMessage->netId) {
              return;
            }

            NetworkManager::Instance().SendAllMessageFromServer<DespawnMessage>(
                despawnMessage);

            Entity* entity = NetworkManager::Instance().GetNetworkEntity(
                despawnMessage->netId);
            if (!entity) {
              return;
            }

            spawnedEntities.remove(entity);
            Entity::Destroy(entity);
          });
}

void DeregisterExampleMessageFunctions() {
  NetworkManager::Instance().UnregisterClientCallback<HandleMessage>(
      exampleClientHandleId);
  NetworkManager::Instance().UnregisterServerCallback<HandleMessage>(
      exampleServerHandleId);
  NetworkManager::Instance().UnregisterClientCallback<SpawnMessage>(
      exampleClientSpawn);
  NetworkManager::Instance().UnregisterClientCallback<SpawnMessage>(
      exampleServerSpawn);
  NetworkManager::Instance().UnregisterClientCallback<DespawnMessage>(
      exampleClientDespawn);
  NetworkManager::Instance().UnregisterClientCallback<DespawnMessage>(
      exampleServerDespawn);
}

void NetworkLevel::OnLevelLoad() {
  // Networking preparation
  RegisterExampleMessageFunctions();

  if (CONFIG_VAL(networkConfig.runServer)) {
    NetworkManager::Instance().CreateServer(
        CONFIG_VAL(networkConfig.defaultServerIP).c_str());
  }

  if (CONFIG_VAL(networkConfig.connectToServer)) {
    NetworkManager::Instance().ConnectToServer(
        CONFIG_VAL(networkConfig.defaultServerIP).c_str(), [](bool b) {
          LOG(Debug::Channel::Networking, "Client connection state: %d", b);
        });
  }

  // Spawn across network
  Input::RegisterKeyPressCallback(KeyCode::Y, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      SpawnMessage* m =
          NetworkManager::Instance().GenerateMessageFromClient<SpawnMessage>();
      m->a = 0;
      NetworkManager::Instance().SendMessageFromClient(m);

      ++spawnCounter;
    }
  });

  // Despawn across network, only the one who spawned it can despawn it
  Input::RegisterKeyPressCallback(KeyCode::H, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      if (despawnCounter > spawnCounter) {
        return;
      }
      DespawnMessage* m = NetworkManager::Instance()
                              .GenerateMessageFromClient<DespawnMessage>();
      m->netId = despawnCounter;
      NetworkManager::Instance().SendMessageFromClient(m);

      ++despawnCounter;
    }
  });

  // Spawn one and set its parent to the latest one spawned by this client
  Input::RegisterKeyPressCallback(KeyCode::U, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      SpawnMessage* m =
          NetworkManager::Instance().GenerateMessageFromClient<SpawnMessage>();
      m->a = spawnedEntities.back()->GetComponent<NetworkId>()->id;
      NetworkManager::Instance().SendMessageFromClient(m);

      ++spawnCounter;
    }
  });

  // Parent to the previously spawned one
  Input::RegisterKeyPressCallback(KeyCode::I, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      auto it = spawnedEntities.end();
      it = std::prev(it);
      it = std::prev(it);
      spawnedEntities.back()
          ->GetComponent<NetworkTransform>()
          ->SetNetworkedParent((*it)->GetComponent<NetworkId>()->id);
    }
  });

  // Unparent
  Input::RegisterKeyPressCallback(KeyCode::K, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      spawnedEntities.back()
          ->GetComponent<NetworkTransform>()
          ->SetNetworkedParentToRoot();
      spawnedEntities.back()->transform->SetLocalScale(
          Math::Vector3(.01, .01, .01));
    }
  });

  Input::RegisterKeyPressCallback(KeyCode::P, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      HandleMessage* handleMessage =
          NetworkManager::Instance().GenerateMessageFromClient<HandleMessage>();
      handleMessage->handle = 0;
      NetworkManager::Instance().SendMessageFromClient(handleMessage);
    }
  });

  Input::RegisterKeyPressCallback(KeyCode::O, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      HandleMessage* handleMessage =
          NetworkManager::Instance().GenerateMessageFromClient<HandleMessage>();
      handleMessage->handle = 1;
      NetworkManager::Instance().SendMessageFromClient(handleMessage);
    }
  });

  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      HandleMessage* handleMessage =
          NetworkManager::Instance().GenerateMessageFromClient<HandleMessage>();
      handleMessage->handle = 2;
      NetworkManager::Instance().SendMessageFromClient(handleMessage);
    }
  });

  // Camera
  Entity* cameraEntity{Entity::CreateEntity("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();

  // Light
  Entity* lightEntity{Entity::CreateEntity("Light")};
  lightEntity->AddComponent<LightComponent>("materials/light.material.xml",
                                            "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightEntity->AddComponent<GridComponent>();
  lightEntity->AddComponent<EditorComponent>();
  lightEntity->AddComponent<EscapeExit>();
}