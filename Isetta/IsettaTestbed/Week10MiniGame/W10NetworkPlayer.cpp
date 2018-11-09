/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/W10NetworkPlayer.h"
#include "Components/NetworkTransform.h"
#include "Custom/IsettaCore.h"
#include "Events/EventObject.h"
#include "Events/Events.h"
#include "Networking/NetworkId.h"

W10NetworkPlayer::W10NetworkPlayer(bool isRight, int swordNetID,
                                   int clientAuthorityID)
    : swordNetId{swordNetID},
      isOnRight{isRight},
      clientAuthorityId{clientAuthorityID} {}

void W10NetworkPlayer::Awake() {
  swordEntity = ADD_ENTITY("Sword");
  swordEntity->GetTransform()->SetParent(GetTransform());
  swordEntity->GetTransform()->SetLocalPos(
      Isetta::Math::Vector3((isOnRight ? 1 : -1) * 0.25f, 0, 0.25f));

  swordEntity->GetTransform()->SetLocalScale(
      Isetta::Math::Vector3{0.375, 0.025, 0.025});
  swordEntity->AddComponent<Isetta::MeshComponent>("primitive/cube.scene.xml");
  auto networkId = swordEntity->AddComponent<Isetta::NetworkId>(swordNetId);
  networkId->clientAuthorityId = clientAuthorityId;
  swordEntity->AddComponent<Isetta::NetworkTransform>();
  Isetta::Events::Instance().RegisterEventListener(
      "Blocked",
      [&](const Isetta::EventObject& eventObject) { SwordBlocked(); });
}

void W10NetworkPlayer::SwordBlocked() {
  swordEntity->GetTransform()->SetParent(nullptr);
}

void W10NetworkPlayer::SwordCollected() {
  swordEntity->GetTransform()->SetParent(GetTransform());
}

void W10NetworkPlayer::Update() {
  if (Isetta::Math::Util::Abs(GetTransform()->GetWorldPos().x -
                              swordEntity->GetTransform()->GetWorldPos().x) <
      0.1f) {
    swordEntity->GetTransform()->SetParent(GetTransform());
  }
}
