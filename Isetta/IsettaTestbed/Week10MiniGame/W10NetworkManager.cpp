/*
 * Copyright (c) 2018 Isetta
 */

#include "Week10MiniGame/W10NetworkManager.h"
#include "Week10MiniGame/W10NetworkPlayer.h"
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
      Isetta::NetworkManager::Instance()
          .SendMessageFromServerToAll<W10SpawnMessage>(
              [potentialClient, this](W10SpawnMessage* message) {
                message->netId = this->netIdBin.GetHandle();
                message->clientAuthorityId = potentialClient;
                message->isOnRight = potentialClient % 2;
                message->swordNetId = this->netIdBin.GetHandle();
              });
    }
  }
}

void W10NetworkManager::HandleSpawnMessage(yojimbo::Message* message) {
  W10SpawnMessage* spawnMessage = reinterpret_cast<W10SpawnMessage*>(message);

  const Isetta::Entity* entity =
      Isetta::NetworkManager::Instance().GetNetworkEntity(spawnMessage->netId);
  if (!entity) {
    Isetta::Events::Instance().RaiseImmediateEvent(
        Isetta::EventObject{"UITextChange", {std::string{"Game Started!"}}});
    Isetta::Entity* e = Isetta::Entity::Instantiate(
        Isetta::Util::StrFormat("Player%d", spawnMessage->netId));
    Isetta::NetworkId* networkId =
        e->AddComponent<Isetta::NetworkId>(spawnMessage->netId);
    networkId->clientAuthorityId = spawnMessage->clientAuthorityId;
    if (networkId->HasClientAuthority()) {
      auto w10Player = e->AddComponent<W10Player>(
          spawnMessage->isOnRight, spawnMessage->swordNetId,
          spawnMessage->clientAuthorityId);
      gameManager->player = w10Player;
    } else {
      auto w10NetworkPlayer = e->AddComponent<W10NetworkPlayer>(
          spawnMessage->isOnRight, spawnMessage->swordNetId,
          spawnMessage->clientAuthorityId);
      gameManager->enemy = w10NetworkPlayer;
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
  if (gameManager->player != nullptr) {
    LOG_INFO(Isetta::Debug::Channel::General, "Client send position report");

    Isetta::NetworkManager::Instance()
        .SendMessageFromClient<W10PositionReportMessage>(
            [this](W10PositionReportMessage* posMessage) {
              posMessage->positionX =
                  this->gameManager->player->transform->GetWorldPos().x;
            });
  }
}

void W10NetworkManager::ServerHandleAttackAttemptMessage(
    int clientIdx, yojimbo::Message* message) {
  lastAttemptClient = clientIdx;
  clientPosX.clear();
  Isetta::NetworkManager::Instance()
      .SendMessageFromServerToAll<W10AttackAttemptMessage>(message);
}

void W10NetworkManager::HandlePositionReport(int clientIdx,
                                             yojimbo::Message* message) {
  const W10PositionReportMessage* posMessage{
      reinterpret_cast<const W10PositionReportMessage*>(message)};
  LOG_INFO(Isetta::Debug::Channel::General, "Handle position report");

  clientPosX.insert_or_assign(clientIdx, posMessage->positionX);
  if (clientPosX.size() == 2) {
    auto distance =
        Isetta::Math::Util::Abs(clientPosX.at(0) - clientPosX.at(1));
    LOG_INFO(Isetta::Debug::Channel::General, "Distance %f", distance);
    int attackSwordPos = clientSwordPos.at(lastAttemptClient);
    for (const auto& swordPos : clientSwordPos) {
      if (swordPos.first != lastAttemptClient) {
        if (swordPos.second != attackSwordPos) {
          if (distance > killDistance) {
            return;
          }
          Isetta::NetworkManager::Instance()
              .SendMessageFromServer<W10AttackResultMessage>(
                  lastAttemptClient, [](W10AttackResultMessage* resultMessage) {
                    resultMessage->result = 0;
                  });

          Isetta::NetworkManager::Instance()
              .SendMessageFromServer<W10AttackResultMessage>(
                  swordPos.first, [](W10AttackResultMessage* resultMessage) {
                    resultMessage->result = 1;
                  });

          clientSwordPos.insert_or_assign(lastAttemptClient, 0);
          clientSwordPos.insert_or_assign(swordPos.first, 0);
          LOG_INFO(Isetta::Debug::Channel::General, "Server: HIT!");
        } else {
          if (distance > blockDistance) {
            return;
          }

          clientSwordPos.insert_or_assign(lastAttemptClient, -1);

          Isetta::NetworkManager::Instance()
              .SendMessageFromServer<W10AttackResultMessage>(
                  lastAttemptClient, [](W10AttackResultMessage* resultMessage) {
                    resultMessage->result = 2;
                  });

          Isetta::NetworkManager::Instance()
              .SendMessageFromServer<W10AttackResultMessage>(
                  swordPos.first, [](W10AttackResultMessage* resultMessage) {
                    resultMessage->result = 3;
                  });

          LOG_INFO(Isetta::Debug::Channel::General, "Server: Not HIT!");
        }
        break;
      }
    }
  }
}

void W10NetworkManager::HandleAttackResultMessage(yojimbo::Message* message) {
  const W10AttackResultMessage* resultMessage{
      reinterpret_cast<const W10AttackResultMessage*>(message)};
  switch (resultMessage->result) {
    case 0:
      LOG_INFO(Isetta::Debug::Channel::General, "Client: I Win ^-^");
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"UITextChange", {std::string{"You Win!"}}});
      Isetta::Events::Instance().RaiseQueuedEvent(
          Isetta::EventObject{"Win", {}});
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"Respawn", {}});
      break;
    case 1:
      LOG_INFO(Isetta::Debug::Channel::General, "Client: I Lose QAQ");
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"UITextChange", {std::string{"You Lose!"}}});
      Isetta::Events::Instance().RaiseQueuedEvent(
          Isetta::EventObject{"Lose", {}});
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"Respawn", {}});
      break;
    case 2:
      LOG_INFO(Isetta::Debug::Channel::General, "Client: I am blocked -A-");
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"Blocked", {}});
      break;
    case 3:
      LOG_INFO(Isetta::Debug::Channel::General,
               "Client: I blocked the other :P");
      Isetta::Events::Instance().RaiseImmediateEvent(
          Isetta::EventObject{"Block", {}});
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

  Isetta::NetworkManager::Instance().RegisterServerCallback<W10CollectMessage>(
      [&](const int clientId, yojimbo::Message* message) {
        clientSwordPos.insert_or_assign(lastAttemptClient, 0);
        Isetta::NetworkManager::Instance()
            .SendMessageFromServerToAllButClient<W10CollectMessage>(clientId,
                                                                    message);
      });

  // Networking connection

  Isetta::Events::Instance().RaiseImmediateEvent(
      Isetta::EventObject{"UITextChange", {std::string{"Waiting!"}}});
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::NUM1, []() {
    Isetta::NetworkManager::Instance().StartHost(
        CONFIG_VAL(networkConfig.defaultServerIP));
    Isetta::Events::Instance().RaiseImmediateEvent(
        Isetta::EventObject{"UITextChange", {std::string{"Started as host!"}}});
  });

  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::NUM2, []() {
    Isetta::NetworkManager::Instance().StartClient(
        CONFIG_VAL(networkConfig.defaultServerIP));
    Isetta::Events::Instance().RaiseImmediateEvent(Isetta::EventObject{
        "UITextChange",
        {Isetta::Util::StrFormat("Connected to %s as client!",
                                 CONFIG_VAL(networkConfig.defaultServerIP))}});
  });

  // if (CONFIG_VAL(networkConfig.runServer)) {
  // Isetta::NetworkManager::Instance().StartServer(
  // CONFIG_VAL(networkConfig.defaultServerIP).c_str());
  // }

  // LOG_INFO(Isetta::Debug::Channel::Networking, "Server running: %d",
  // Isetta::NetworkManager::Instance().ServerIsRunning());

  // if (CONFIG_VAL(networkConfig.connectToServer)) {
  // Isetta::NetworkManager::Instance().StartClient(
  // CONFIG_VAL(networkConfig.defaultServerIP).c_str(), [](bool b) {
  // LOG_INFO(Isetta::Debug::Channel::Networking,
  // "Client connection state: %d", b);
  // });
  // }

  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::R, []() {
    Isetta::Events::Instance().RaiseImmediateEvent(
        Isetta::EventObject{"UITextChange", {std::string{"Ready!"}}});
    Isetta::NetworkManager::Instance().SendMessageFromClient<W10ReadyMessage>(
        [](W10ReadyMessage* message) {});
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
      Isetta::LevelManager::Instance().loadedLevel->GetEntityByName(
          "GameManager");
  if (gameManagerEntity == nullptr) {
    LOG_WARNING(Isetta::Debug::Channel::General,
                "No GameManager entity found.");
  } else {
    gameManager = gameManagerEntity->GetComponent<W10GameManager>();
  }
}
