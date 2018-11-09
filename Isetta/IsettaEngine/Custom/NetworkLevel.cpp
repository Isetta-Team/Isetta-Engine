/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/NetworkLevel.h"

#include "Components/NetworkTransform.h"
#include "Components/FlyController.h"
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Core/Math/Math.h"
#include "Custom/KeyTransform.h"
#include "Custom/OscillateMove.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Input/Input.h"
#include "Networking/NetworkId.h"
#include "Scene/Entity.h"
#include "Components/GridComponent.h"
#include "Components/Editor/Editor.h"

using namespace Isetta;

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

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
              AudioSource audio = AudioSource();
              audio.SetAudioClip("gunshot.aiff");
              audio.Play(false, 1.f);
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
            const Entity* entity = NetworkManager::Instance().GetNetworkEntity(
                spawnMessage->netId);
            if (!entity) {
              Entity* e = LevelManager::Instance().currentLevel->AddEntity(
                  Util::StrFormat("NetworkEntity%d", spawnMessage->netId));
              NetworkId* netId = e->AddComponent<NetworkId>(spawnMessage->netId);
              netId->clientAuthorityId = spawnMessage->clientAuthorityId;
              spawnedEntities.push_back(e);

              // Zomble
              e->GetTransform()->SetLocalScale(Math::Vector3::one * .01);
              MeshComponent* mesh = e->AddComponent<MeshComponent, true>(
                  "Zombie/Zombie.scene.xml");
              AnimationComponent* animation =
                  e->AddComponent<AnimationComponent, true>(mesh);
              animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
              e->GetComponent<AnimationComponent>()->Play();
              if (netId->HasClientAuthority()) {
                e->AddComponent<KeyTransform>();
              }
              e->AddComponent<NetworkTransform>();
            }
          });

  exampleServerSpawn =
      NetworkManager::Instance().RegisterServerCallback<SpawnMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            static int count = 1;
            SpawnMessage* spawnMessage =
                reinterpret_cast<SpawnMessage*>(message);

            if (!spawnMessage->netId) {
              Entity* e = LevelManager::Instance().currentLevel->AddEntity(
                  Util::StrFormat("NetworkEntity%d", count++));
              NetworkId* netId = e->AddComponent<NetworkId>();
              netId->clientAuthorityId = clientIdx;
              spawnedEntities.push_back(e);
              spawnMessage->netId = netId->id;
              spawnMessage->clientAuthorityId = clientIdx;

              // Zomble
              e->GetTransform()->SetLocalScale(Math::Vector3::one * .01);
              MeshComponent* mesh = e->AddComponent<MeshComponent, true>(
                  "Zombie/Zombie.scene.xml");
              AnimationComponent* animation =
                  e->AddComponent<AnimationComponent, true>(mesh);
              animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
              e->GetComponent<AnimationComponent>()->Play();
              if (netId->HasClientAuthority()) {
                e->AddComponent<KeyTransform>();
              }
              e->AddComponent<NetworkTransform>();
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
            NetworkManager::Instance().RemoveNetworkId(
                entity->GetComponent<NetworkId>());
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

            NetworkManager::Instance().RemoveNetworkId(
                entity->GetComponent<NetworkId>());
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

void NetworkLevel::LoadLevel() {
  // Networking preparation
  RegisterExampleMessageFunctions();

  if (Config::Instance().networkConfig.runServer.GetVal()) {
    NetworkManager::Instance().CreateServer(
        Config::Instance().networkConfig.defaultServerIP.GetVal().c_str());
  }
  if (Config::Instance().networkConfig.connectToServer.GetVal()) {
    NetworkManager::Instance().ConnectToServer(
        Config::Instance().networkConfig.defaultServerIP.GetVal().c_str(),
        [](bool b) {
          LOG(Debug::Channel::Networking, "Client connection state: %d", b);
        });
  }

  Input::RegisterKeyPressCallback(KeyCode::Y, []() {
    if (NetworkManager::Instance().LocalClientIsConnected()) {
      SpawnMessage* m =
          NetworkManager::Instance().GenerateMessageFromClient<SpawnMessage>();
      m->a = 1;
      m->b = 2;
      m->c = 3;
      NetworkManager::Instance().SendMessageFromClient(m);

      ++spawnCounter;
    }
  });
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
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  // Light
  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);
  lightEntity->AddComponent<GridComponent>();
  lightEntity->AddComponent<Editor>();
}