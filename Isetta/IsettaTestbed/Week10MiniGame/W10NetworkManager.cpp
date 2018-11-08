/*
 * Copyright (c) 2018 Isetta
 */

#include "Week10MiniGame/W10NetworkManager.h"
#include "Components/NetworkTransform.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Events/Events.h"
#include "Networking/NetworkId.h"
#include "Week10MiniGame/W10Player.h"

void W10NetworkManager::HandleReadyMessage(int clientIdx,
                                           yojimbo::Message* message) {
  if (clientSwordPos.count(clientIdx) == 0) {
    clientCount++;
    clientSwordPos.insert({clientIdx, 0});
  }

  if (clientCount == 2) {
    for (const auto& swordPair : clientSwordPos) {
      int potentialClient = swordPair.first;
      W10SpawnMessage* spawn =
          Isetta::NetworkManager::Instance()
              .GenerateMessageFromServer<W10SpawnMessage>(potentialClient);
      spawn->netId = Isetta::NetworkManager::Instance().CreateNetId();
      spawn->clientAuthorityId = potentialClient;
      spawn->isOnRight = potentialClient % 2;
      spawn->swordNetId = Isetta::NetworkManager::Instance().CreateNetId();

      Isetta::NetworkManager::Instance()
          .SendAllMessageFromServer<W10SpawnMessage>(spawn);
    }
  }
}

void W10NetworkManager::HandleSpawnMessage(yojimbo::Message* message) {
  W10SpawnMessage* spawnMessage = reinterpret_cast<W10SpawnMessage*>(message);

  const Isetta::Entity* entity =
      Isetta::NetworkManager::Instance().GetNetworkEntity(spawnMessage->netId);
  if (!entity) {
    Isetta::Entity* e =
        ADD_ENTITY(Isetta::Util::StrFormat("Player%d", spawnMessage->netId));
    Isetta::NetworkId* networkId =
        e->AddComponent<Isetta::NetworkId>(spawnMessage->netId);
    networkId->clientAuthorityId = spawnMessage->clientAuthorityId;

    e->AddComponent<Isetta::MeshComponent>("primitive/cube.scene.xml");
    e->SetTransform(
        Isetta::Math::Vector3{spawnMessage->isOnRight ? -1.f : 1.f, 0, 0},
        Isetta::Math::Vector3::zero, Isetta::Math::Vector3{0.25, 0.5, 0.25});

    if (networkId->HasClientAuthority()) {
      auto w10Player = e->AddComponent<W10Player>(
          spawnMessage->isOnRight, spawnMessage->swordNetId,
          spawnMessage->clientAuthorityId);
      gameManager->player = w10Player;
    } else {
      auto swordEntity = ADD_ENTITY("Sword");
      LOG_INFO(Isetta::Debug::Channel::General, "Sword network id: %d",
               spawnMessage->swordNetId);
      swordEntity->GetTransform()->SetParent(e->GetTransform());
      auto networkId = swordEntity->AddComponent<Isetta::NetworkId>(
          spawnMessage->swordNetId);
      networkId->clientAuthorityId = spawnMessage->clientAuthorityId;
      swordEntity->AddComponent<Isetta::NetworkTransform>();
    }
    e->AddComponent<Isetta::NetworkTransform>();
  }
}

void W10NetworkManager::HandleSwordPosMessage(int clientIdx,
                                              yojimbo::Message* message) {
  W10SwordPosMessage* swordMessage{
      reinterpret_cast<W10SwordPosMessage*>(message)};
  clientSwordPos.insert_or_assign(clientIdx, swordMessage->swordPos);
  LOG_INFO(Isetta::Debug::Channel::General, "Client: %d moves sword to %d",
           clientIdx, swordMessage->swordPos);
}

void W10NetworkManager::ClientHandleAttackAttemptMessage(
    yojimbo::Message* message) {
  W10PositionReportMessage* posMessage =
      Isetta::NetworkManager::Instance()
          .GenerateMessageFromClient<W10PositionReportMessage>();
  if (gameManager->player != nullptr) {
    posMessage->positionX =
        gameManager->player->GetTransform()->GetWorldPos().x;
    Isetta::NetworkManager::Instance().SendMessageFromClient(posMessage);
  }
}

void W10NetworkManager::ServerHandleAttackAttemptMessage(
    int clientIdx, yojimbo::Message* message) {
  lastAttemptClient = clientIdx;
  clientPosX.clear();
  Isetta::NetworkManager::Instance()
      .SendAllMessageFromServer<W10AttackAttemptMessage>(message);
}

void W10NetworkManager::HandlePositionReport(int clientIdx,
                                             yojimbo::Message* message) {
  const W10PositionReportMessage* posMessage{
      reinterpret_cast<const W10PositionReportMessage*>(message)};
  clientPosX.insert_or_assign(clientIdx, posMessage->positionX);
  if (clientPosX.size() == 2) {
    auto distance = Isetta::Math::Util::Abs(clientPosX.at(0) - clientPosX.at(1));
    LOG_INFO(Isetta::Debug::Channel::General, "Distance %f", distance);

    // for test
    int attackSwordPos = clientSwordPos.at(lastAttemptClient);
    for (const auto& swordPos : clientSwordPos) {
      if (swordPos.first != lastAttemptClient) {
        if (swordPos.second != attackSwordPos) {
          W10AttackResultMessage* resultMessage1{
              Isetta::NetworkManager::Instance()
                  .GenerateMessageFromServer<W10AttackResultMessage>(
                      lastAttemptClient)};
          W10AttackResultMessage* resultMessage2{
              Isetta::NetworkManager::Instance()
                  .GenerateMessageFromServer<W10AttackResultMessage>(
                      swordPos.first)};
          resultMessage1->result = 0;
          resultMessage2->result = 1;
          Isetta::NetworkManager::Instance().SendMessageFromServer(
              lastAttemptClient, resultMessage1);
          Isetta::NetworkManager::Instance().SendMessageFromServer(
              swordPos.first, resultMessage2);
          LOG_INFO(Isetta::Debug::Channel::General, "Server: HIT!");
        } else {
          W10AttackResultMessage* resultMessage{
              Isetta::NetworkManager::Instance()
                  .GenerateMessageFromServer<W10AttackResultMessage>(
                      lastAttemptClient)};
          resultMessage->result = 2;
          Isetta::NetworkManager::Instance().SendMessageFromServer(
              lastAttemptClient, resultMessage);
          LOG_INFO(Isetta::Debug::Channel::General, "Server: Not HIT!");
        }
      }
    }
    // TODO(Chaojie): real game logic
  }
}

void W10NetworkManager::HandleAttackResultMessage(yojimbo::Message* message) {
  const W10AttackResultMessage* resultMessage{
      reinterpret_cast<const W10AttackResultMessage*>(message)};
  switch (resultMessage->result) {
    case 0:
      LOG_INFO(Isetta::Debug::Channel::General, "Client: I Win ^-^");
      break;
    case 1:
      LOG_INFO(Isetta::Debug::Channel::General, "Client: I Lose QAQ");
      break;
    case 2:
      LOG_INFO(Isetta::Debug::Channel::General, "Client: I am blocked -A-");
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"Blocked", {}});
      break;
    default:
      LOG_WARNING(Isetta::Debug::Channel::General,
                  "Wrong attack result message!");
  }
}

void W10NetworkManager::Awake() {
  readyHandle = Isetta::NetworkManager::Instance()
                    .RegisterServerCallback<W10ReadyMessage>(
                        [&](int clientId, yojimbo::Message* message) {
                          HandleReadyMessage(clientId, message);
                        });
  spawnHandle =
      Isetta::NetworkManager::Instance()
          .RegisterClientCallback<W10SpawnMessage>(
              [&](yojimbo::Message* message) { HandleSpawnMessage(message); });

  swordHandle = Isetta::NetworkManager::Instance()
                    .RegisterServerCallback<W10SwordPosMessage>(
                        [&](int clientId, yojimbo::Message* message) {
                          HandleSwordPosMessage(clientId, message);
                        });
  clientAttackHandle = Isetta::NetworkManager::Instance()
                           .RegisterClientCallback<W10AttackAttemptMessage>(
                               [&](yojimbo::Message* message) {
                                 ClientHandleAttackAttemptMessage(message);
                               });
  serverAttackHandle =
      Isetta::NetworkManager::Instance()
          .RegisterServerCallback<W10AttackAttemptMessage>(
              [&](int clientIdx, yojimbo::Message* message) {
                ServerHandleAttackAttemptMessage(clientIdx, message);
              });
  positionHandle = Isetta::NetworkManager::Instance()
                       .RegisterServerCallback<W10PositionReportMessage>(
                           [&](int clientIdx, yojimbo::Message* message) {
                             HandlePositionReport(clientIdx, message);
                           });
  resultHandle = Isetta::NetworkManager::Instance()
                     .RegisterClientCallback<W10AttackResultMessage>(
                         [&](yojimbo::Message* message) {
                           HandleAttackResultMessage(message);
                         });

  if (Isetta::Config::Instance().networkConfig.runServer.GetVal()) {
    Isetta::NetworkManager::Instance().CreateServer(
        Isetta::Config::Instance()
            .networkConfig.defaultServerIP.GetVal()
            .c_str());
  }
  LOG_INFO(Isetta::Debug::Channel::Networking, "Server running: %d",
           Isetta::NetworkManager::Instance().ServerIsRunning());

  if (Isetta::Config::Instance().networkConfig.connectToServer.GetVal()) {
    Isetta::NetworkManager::Instance().ConnectToServer(
        Isetta::Config::Instance()
            .networkConfig.defaultServerIP.GetVal()
            .c_str(),
        [](bool b) {
          LOG_INFO(Isetta::Debug::Channel::Networking,
                   "Client connection state: %d", b);
        });
  }

  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::Y, []() {
    W10ReadyMessage* m = Isetta::NetworkManager::Instance()
                             .GenerateMessageFromClient<W10ReadyMessage>();
    Isetta::NetworkManager::Instance().SendMessageFromClient(m);
  });

  // // For debug use
  // Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::NUM1, [&]() {
  //   clientSwordPos.insert_or_assign(2, 1);
  //   LOG_INFO(Isetta::Debug::Channel::General, "Test Sword is on 1");
  // });
  // Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::NUM2, [&]() {
  //   clientSwordPos.insert_or_assign(2, 0);
  //   LOG_INFO(Isetta::Debug::Channel::General, "Test Sword is on 0");
  // });
  // Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::NUM3, [&]() {
  //   clientSwordPos.insert_or_assign(2, -1);
  //   LOG_INFO(Isetta::Debug::Channel::General, "Test Sword is on -1");
  // });
}

void W10NetworkManager::Start() {
  auto gameManagerEntity =
      Isetta::LevelManager::Instance().currentLevel->GetEntityByName(
          "GameManager");
  if (gameManagerEntity == nullptr) {
    LOG_WARNING(Isetta::Debug::Channel::General,
                "No GameManager entity found.");
  } else {
    gameManager = gameManagerEntity->GetComponent<W10GameManager>();
  }
}
