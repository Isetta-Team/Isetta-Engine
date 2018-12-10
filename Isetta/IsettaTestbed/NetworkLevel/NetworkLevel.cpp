/*
 * Copyright (c) 2018 Isetta
 */
#include "NetworkLevel.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/Editor/NetworkMonitor.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/EscapeExit.h"
#include "Custom/KeyTransform.h"
#include "NetworkLevel/NetworkTestComp.h"

using namespace Isetta;

// Counters to use for spawning and despawning
// (not effective over the network though!)
int spawnCounter = 0;
int despawnCounter = 0;
std::list<Entity*> spawnedEntities;

// Handles for deregistering our handling functions
U16 exampleClientHandleId;
U16 exampleServerHandleId;
U16 exampleClientStringId;
U16 exampleServerStringId;

U16 exampleClientSpawn;
U16 exampleServerSpawn;
U16 exampleClientDespawn;
U16 exampleServerDespawn;

// Registers all of our message handling functions
void RegisterExampleMessageFunctions() {
  // Receiving and responding to a HandleMessage on the client
  exampleClientHandleId =
      NetworkManager::Instance().RegisterClientCallback<HandleMessage>(
          [](yojimbo::Message* message) {
            // We'll always have to cast our messasge into the message we're
            // anticipating
            HandleMessage* handleMessage = static_cast<HandleMessage*>(message);

            LOG(Debug::Channel::Networking, "Server sends handle #%d",
                handleMessage->handle);

            // Depending on the handle, we can do something (we tore out a
            // couple system here though)
            if (handleMessage->handle == 0) {
              LOG(Debug::Channel::Networking,
                  "Server says we should play the animation!");
            }
            if (handleMessage->handle == 1) {
              LOG(Debug::Channel::Networking,
                  "Server says we should stop the animation!");
            }
          });

  // Receiving and responding to a HandleMessage on the server
  exampleServerHandleId =
      NetworkManager::Instance().RegisterServerCallback<HandleMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            HandleMessage* handleMessage =
                reinterpret_cast<HandleMessage*>(message);

            LOG(Debug::Channel::Networking, "Client %d sends handle #%d",
                clientIdx, handleMessage->handle);

            // We can relay the handle message out to all of the clients
            // using a helper function with NetworkManager
            NetworkManager::Instance()
                .SendMessageFromServerToAll<HandleMessage>(handleMessage);
          });

  // Receiving and responding to a SpawnMessage on the client
  exampleClientSpawn =
      NetworkManager::Instance().RegisterClientCallback<SpawnMessage>(
          [](yojimbo::Message* message) {
            // If we're the host, then we don't want to handle the message
            if (NetworkManager::Instance().IsServerRunning()) {
              return;
            }

            SpawnMessage* spawnMessage =
                reinterpret_cast<SpawnMessage*>(message);

            // Try to find if the network ID we've been sent has already
            // been used to spawn something
            Entity* entity = NetworkManager::Instance().GetNetworkEntity(
                spawnMessage->netId);

            // If the entity has not been spawned yet, then we should spawn one!
            if (!entity) {
              Entity* e = Entity::Instantiate(
                  Util::StrFormat("NetworkEntity (%d)", spawnMessage->netId));

              // With the spawned entity, we should attach a network ID with the
              // integer that we were sent
              NetworkId* netId =
                  e->AddComponent<NetworkId>(spawnMessage->netId);
              // We should set the authority to the appropriate client
              netId->clientAuthorityId = spawnMessage->clientAuthorityId;

              spawnedEntities.push_back(e);

              // Zombie
              e->transform->SetLocalScale(
                  Math::Vector3::one * .01);  // The model is a bit too big...
              MeshComponent* mesh = e->AddComponent<MeshComponent, true>(
                  "Halves/Zombie/Zombie.scene.xml");
              // If we have client authority, then we want to be able to control
              // the zombie
              if (netId->HasClientAuthority()) {
                e->AddComponent<KeyTransform>();
              }
              // The NetworkTransform allows the zombie to get matched on
              // computers that are across the network runnig their own games
              e->AddComponent<NetworkTransform>();

              // If the spawnMessage has a parent, then we need to parent our
              // spawned entity with the corresponding entity
              if (spawnMessage->parentId) {
                Entity* parent = NetworkManager::Instance().GetNetworkEntity(
                    spawnMessage->parentId);
                e->transform->SetParent(parent->transform);
                e->transform->SetLocalScale(Math::Vector3::one);
              }
            }
          });

  // Receiving and responding to a SpawnMessage on the server
  exampleServerSpawn =
      NetworkManager::Instance().RegisterServerCallback<SpawnMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            static int count = 1;  // Easy way to keep track of our spawn count
            SpawnMessage* spawnMessage =
                reinterpret_cast<SpawnMessage*>(message);

            // If we receive a SpawnMessage that has a networkd ID of 0, then
            // we should spawn the entity and assign a new network ID to it
            if (!spawnMessage->netId) {
              Entity* e = Entity::Instantiate(
                  Util::StrFormat("NetworkEntity%d", count++));
              // Creating a network ID without passing an integer in will
              // generate one for us
              NetworkId* netId = e->AddComponent<NetworkId>();
              netId->clientAuthorityId = clientIdx;  // Set the client authority
                                                     // to whoever told us to
                                                     // spawn this zombie
              spawnedEntities.push_back(e);

              // Populate our received SpawnMessage with the ID and client
              // authority for when we use it to send out messages later
              spawnMessage->netId = netId->id;
              spawnMessage->clientAuthorityId = clientIdx;

              // Zombie again
              e->transform->SetLocalScale(Math::Vector3::one * .01);
              MeshComponent* mesh = e->AddComponent<MeshComponent, true>(
                  "Halves/Zombie/Zombie.scene.xml");
              if (netId->HasClientAuthority()) {
                e->AddComponent<KeyTransform>();
              }
              e->AddComponent<NetworkTransform>();

              if (spawnMessage->parentId) {
                Entity* parent = NetworkManager::Instance().GetNetworkEntity(
                    spawnMessage->parentId);

                e->transform->SetParent(parent->transform);
                e->transform->SetLocalScale(Math::Vector3::one);
              }
            }

            NetworkManager::Instance().SendMessageFromServerToAll<SpawnMessage>(
                spawnMessage);
          });

  // Receiving and responding to a DespawnMessage on the client
  exampleClientDespawn =
      NetworkManager::Instance().RegisterClientCallback<DespawnMessage>(
          [](yojimbo::Message* message) {
            // Again, if we're the host, then we don't want to run the client
            // callback
            if (NetworkManager::Instance().IsServerRunning()) {
              return;
            }

            DespawnMessage* despawnMessage =
                reinterpret_cast<DespawnMessage*>(message);

            // Try to get the entity matching the ID that we were just sent
            Entity* entity = NetworkManager::Instance().GetNetworkEntity(
                despawnMessage->netId);
            // If the entity doesn't exist, then we don't need to worry about
            // despawning it!
            if (!entity) {
              return;
            }

            spawnedEntities.remove(entity);
            Entity::Destroy(entity);
          });

  // Receiving and responding to a DespawnMessage on the server
  exampleServerDespawn =
      NetworkManager::Instance().RegisterServerCallback<DespawnMessage>(
          [](int clientIdx, yojimbo::Message* message) {
            DespawnMessage* despawnMessage =
                reinterpret_cast<DespawnMessage*>(message);

            // If the DespawnMessage doesn't have a network ID, then we
            // can't really despawn anything...
            if (!despawnMessage->netId) {
              return;
            }

            NetworkManager::Instance()
                .SendMessageFromServerToAll<DespawnMessage>(despawnMessage);

            // After sending out the DespawnMessage, we need to despawn
            // the entity on our own machine (because we're a host)
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

void NetworkLevel::Load() {
  // Networking preparation
  RegisterExampleMessageFunctions();

  // Spawn across network
  Input::RegisterKeyPressCallback(KeyCode::Y, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().SendMessageFromClient<SpawnMessage>(
          [](SpawnMessage* message) { message->parentId = 0; });
      ++spawnCounter;
    }
  });

  // Despawn across network, only the one who spawned it can despawn it
  Input::RegisterKeyPressCallback(KeyCode::H, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      if (despawnCounter > spawnCounter) {
        return;
      }
      NetworkManager::Instance().SendMessageFromClient<DespawnMessage>(
          [](DespawnMessage* message) { message->netId = despawnCounter; });

      ++despawnCounter;
    }
  });

  // Spawn one and set its parent to the latest one spawned by this client
  Input::RegisterKeyPressCallback(KeyCode::U, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().SendMessageFromClient<SpawnMessage>(
          [](SpawnMessage* message) {
            message->parentId =
                spawnedEntities.back()->GetComponent<NetworkId>()->id;
          });

      ++spawnCounter;
    }
  });

  // Parent to the previously spawned one
  Input::RegisterKeyPressCallback(KeyCode::I, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      auto it = spawnedEntities.end();
      it = std::prev(it);
      it = std::prev(it);
      spawnedEntities.back()
          ->GetComponent<NetworkTransform>()
          ->SetNetworkedParent((*it)->GetComponent<NetworkId>()->id);
    }
  });

  // Unparent the previously spawned one
  Input::RegisterKeyPressCallback(KeyCode::K, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      spawnedEntities.back()
          ->GetComponent<NetworkTransform>()
          ->SetNetworkedParentToRoot();
      spawnedEntities.back()->transform->SetLocalScale(
          Math::Vector3(.01, .01, .01));
    }
  });

  // Send out a generic handle message with 0
  Input::RegisterKeyPressCallback(KeyCode::P, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().SendMessageFromClient<HandleMessage>(
          [](HandleMessage* message) { message->handle = 0; });
    }
  });

  // Send out a generic handle message with 1
  Input::RegisterKeyPressCallback(KeyCode::O, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().SendMessageFromClient<HandleMessage>(
          [](HandleMessage* message) { message->handle = 1; });
    }
  });

  // Send out a generic handle message with 2 (this one won't be responded
  // to!)
  Input::RegisterMousePressCallback(MouseButton::LEFT, []() {
    if (NetworkManager::Instance().IsClientRunning()) {
      NetworkManager::Instance().SendMessageFromClient<HandleMessage>(
          [](HandleMessage* message) { message->handle = 2; });
    }
  });

  // Camera
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->AddComponent<AudioListener>();
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  // Light
  Entity* lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* debugEntity = Entity::Instantiate("Debug");
  debugEntity->AddComponent<GridComponent>();
  debugEntity->AddComponent<EditorComponent>();
  debugEntity->AddComponent<EscapeExit>();
  debugEntity->AddComponent<NetworkDiscovery>();
  debugEntity->AddComponent<NetworkMonitor>();  // Draw network state as UI
  debugEntity->AddComponent<NetworkTestComp>();
}