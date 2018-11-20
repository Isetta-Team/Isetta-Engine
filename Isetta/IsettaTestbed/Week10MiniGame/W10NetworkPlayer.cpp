/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/W10NetworkPlayer.h"
#include "Networking/NetworkTransform.h"
#include "Custom/IsettaCore.h"
#include "Events/EventObject.h"
#include "Events/Events.h"
#include "Networking/NetworkId.h"
#include "W10NetworkManager.h"

void W10NetworkPlayer::InitPosition() {
  entity->SetTransform(Isetta::Math::Vector3{isOnRight ? -1.f : 1.f, 0, 0},
                            Isetta::Math::Vector3::zero,
                            Isetta::Math::Vector3{1, 1, 1});

  swordEntity->transform->SetParent(transform);
  swordEntity->transform->SetLocalPos(
      Isetta::Math::Vector3((isOnRight ? 1 : -1) * 0.25f, 0, 0.25f));

  swordEntity->transform->SetLocalScale(
      Isetta::Math::Vector3{0.375, 0.025, 0.025});
}

W10NetworkPlayer::W10NetworkPlayer(bool isRight, int swordNetID,
                                   int clientAuthorityID)
    : swordNetId{swordNetID},
      isOnRight{isRight},
      clientAuthorityId{clientAuthorityID} {}

void W10NetworkPlayer::Awake() {
  entity->AddComponent<Isetta::MeshComponent>(
      "blockFencing/Enemy.scene.xml");

  swordEntity = ADD_ENTITY("Sword2");
  swordEntity->AddComponent<Isetta::MeshComponent>("primitive/cube.scene.xml");
  auto networkId = swordEntity->AddComponent<Isetta::NetworkId>(swordNetId);
  networkId->clientAuthorityId = clientAuthorityId;
  swordEntity->AddComponent<Isetta::NetworkTransform>();
  Isetta::Events::Instance().RegisterEventListener(
      "Block", [&](const Isetta::EventObject& eventObject) { SwordBlocked(); });
  Isetta::NetworkManager::Instance().RegisterClientCallback<W10CollectMessage>(
      [&](yojimbo::Message* message) {
        W10CollectMessage* collectMessage =
            reinterpret_cast<W10CollectMessage*>(message);
        LOG_INFO(Isetta::Debug::Channel::General,
                 "Receive Collect Message: %d -> %d",
                 collectMessage->swordNetId, swordNetId);
      });
  Isetta::Events::Instance().RegisterEventListener(
      "Respawn",
      [&](const Isetta::EventObject& eventObject) { InitPosition(); });
  InitPosition();
}

void W10NetworkPlayer::SwordBlocked() {
  LOG_INFO(Isetta::Debug::Channel::General, "Sword blocked");

  swordEntity->transform->SetParent(nullptr);
}

void W10NetworkPlayer::Update() {
  // if (swordEntity->transform->GetWorldPos().y < 0) {
  //   if (Isetta::Math::Util::Abs(transform->GetWorldPos().x -
  //                               swordEntity->transform->GetWorldPos().x)
  //                               <
  //       0.1f) {
  //     swordEntity->transform->SetParent(transform);
  //   }
  // }
}
