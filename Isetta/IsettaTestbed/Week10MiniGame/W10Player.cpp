/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/W10Player.h"
#include "W10NetworkManager.h"

W10Player::W10Player(const bool isRight, const int swordNetID,
                     const int clientAuthorityID)
    : swordEntity{nullptr},
      isOnRight{isRight},
      canOperate{true},
      horizontalSpeed{1},
      swordPos{0},
      swordTargetX{0.5},
      swordXProgress{0},
      stabSpeed{7},
      swordStabStatus{0},
      gravity(9.8),
      flyDuration{0},
      totalFlyDuration(0),
      targetX(0),
      targetY(-0.25),
      originY(0),
      v0x{isRight ? -2.f : 2.f},
      v0y(0),
      isSwordFlying(false),
      clientAuthorityId(clientAuthorityID),
      swordNetId(swordNetID) {}

void W10Player::Awake() {
  entity->AddComponent<Isetta::MeshComponent>("Week10/Player.scene.xml");
  swordEntity = Isetta::Primitive::Create(Isetta::Primitive::Type::Cube);
  auto networkId = swordEntity->AddComponent<Isetta::NetworkId>(swordNetId);
  networkId->clientAuthorityId = clientAuthorityId;
  swordEntity->AddComponent<Isetta::NetworkTransform>();

  Isetta::Events::Instance().RegisterEventListener(
      "Blocked",
      [&](const Isetta::EventObject& eventObject) { SwordBlocked(); });
  Isetta::Events::Instance().RegisterEventListener(
      "Respawn",
      [&](const Isetta::EventObject& eventObject) { InitPosition(); });
  Isetta::Events::Instance().RegisterEventListener(
      "RegainInput", [&](const Isetta::EventObject& eventObject) {
        Isetta::Events::Instance().RaiseQueuedEvent(Isetta::EventObject{
            "UITextChange", {std::string{"Game Started!"}}});
        canOperate = true;
      });

  InitPosition();
}

void W10Player::Start() {
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::UP_ARROW, [&]() {
    if (canOperate) ChangeSwordVerticlePosition(1);
  });
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::DOWN_ARROW, [&]() {
    if (canOperate) ChangeSwordVerticlePosition(-1);
  });
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::SPACE, [&]() {
    if (canOperate && swordStabStatus == 0) swordStabStatus = 1;
  });
}

void W10Player::Update() {
  float direction{0};
  if (canOperate && Isetta::Input::IsKeyPressed(Isetta::KeyCode::A)) {
    direction -= 1;
  }
  if (canOperate && Isetta::Input::IsKeyPressed(Isetta::KeyCode::D)) {
    direction += 1;
  }
  transform->TranslateWorld(direction * horizontalSpeed *
                            Isetta::Time::GetDeltaTime() *
                            Isetta::Math::Vector3::left);

  ChangeSwordHorizontalPosition(Isetta::Time::GetDeltaTime());
  if (swordStabStatus == 0) isSwordFlying = false;

  if (!isSwordFlying && swordStabStatus == 3) {
    if (Isetta::Math::Util::Abs(transform->GetWorldPos().x -
                                swordEntity->transform->GetWorldPos().x) <
        0.1f) {
      swordEntity->GetComponent<Isetta::NetworkTransform>()->SetNetworkedParent(
          entity->GetComponent<Isetta::NetworkId>()->id);
      swordEntity->transform->SetLocalPos(
          Isetta::Math::Vector3((isOnRight ? 1 : -1) * 0.25f, 0, 0.25f));
      swordStabStatus = 0;
      swordPos = 0;
      Isetta::NetworkManager::Instance()
          .SendMessageFromClient<W10CollectMessage>(
              [this](W10CollectMessage* message) {
                message->swordNetId = this->swordNetId;
              });
      swordEntity->GetComponent<Isetta::NetworkTransform>()->ForceSendTransform(
          true);
    }
  }

  if (isSwordFlying) {
    flyDuration += Isetta::Time::GetDeltaTime();
    if (flyDuration > totalFlyDuration) {
      swordEntity->transform->SetWorldPos(
          Isetta::Math::Vector3{targetX, targetY, 0});
      isSwordFlying = false;
      flyDuration = 0;
    } else {
      float y = originY + v0y * flyDuration -
                0.5 * gravity * flyDuration * flyDuration;
      float x = targetX - (totalFlyDuration - flyDuration) * v0x;
      swordEntity->transform->SetWorldPos(Isetta::Math::Vector3{x, y, 0});
    }
  }
}

void W10Player::InitPosition() {
  entity->SetTransform(Isetta::Math::Vector3{isOnRight ? -1.f : 1.f, 0, 0},
                       Isetta::Math::Vector3::zero,
                       Isetta::Math::Vector3{1, 1, 1});

  swordEntity->transform->SetParent(transform);
  swordEntity->transform->SetLocalPos(
      Isetta::Math::Vector3((isOnRight ? 1 : -1) * 0.25f, 0, 0.25f));

  swordEntity->transform->SetLocalScale(
      Isetta::Math::Vector3{0.375, 0.025, 0.025});
  swordStabStatus = 0;
  swordPos = 0;
  canOperate = false;
  Isetta::Events::Instance().RaiseQueuedEvent(
      Isetta::EventObject{"RegainInput",
                          Isetta::Time::GetFrameCount() + 200,
                          Isetta::EventPriority::MEDIUM,
                          {}});
}

void W10Player::ChangeSwordVerticlePosition(int direction) {
  if (swordStabStatus != 0) return;
  swordPos += direction;
  swordPos = Isetta::Math::Util::Clamp(-1, 1, swordPos);

  Isetta::NetworkManager::Instance().SendMessageFromClient<W10SwordPosMessage>(
      [this](W10SwordPosMessage* swordMessage) {
        swordMessage->swordPos = this->swordPos;
      });

  auto swordLocalPos = swordEntity->transform->GetLocalPos();
  swordLocalPos.y = swordPos * 0.15;
  swordEntity->transform->SetLocalPos(swordLocalPos);
}

void W10Player::ChangeSwordHorizontalPosition(float deltaTime) {
  int sign = 0;
  if (swordStabStatus == 0 || swordStabStatus == 3) return;
  if (swordStabStatus == 1) {
    sign = 1;
  } else if (swordStabStatus == 2) {
    sign = -1;
  }

  swordXProgress += sign * stabSpeed * deltaTime;
  swordXProgress = Isetta::Math::Util::Clamp01(swordXProgress);
  auto swordLocalPos = swordEntity->transform->GetLocalPos();
  swordLocalPos.x = Isetta::Math::Util::Lerp(
      (isOnRight ? 1 : -1) * 0.25, (isOnRight ? 1 : -1) * swordTargetX,
      swordXProgress);
  swordEntity->transform->SetLocalPos(swordLocalPos);

  if (swordXProgress == 1) {
    // to revoke
    Isetta::NetworkManager::Instance()
        .SendMessageFromClient<W10AttackAttemptMessage>(
            [](W10AttackAttemptMessage* attackMessage) {});

    swordStabStatus = 2;
  }
  if (swordXProgress == 0) {
    // to idle
    swordStabStatus = 0;
  }
}

void W10Player::SwordBlocked() {
  if (swordStabStatus != 2) return;
  swordStabStatus = 3;
  float randomX;
  float currentX{transform->GetWorldPos().x};
  if (isOnRight) {
    randomX = -2 +
              Isetta::Math::Random::GetRandom01() * (currentX - -2) / 3 * 2 +
              (currentX - -2) / 3;
  } else {
    randomX = 2 + Isetta::Math::Random::GetRandom01() * (currentX - 2) / 3 * 2 +
              (currentX - 2) / 3;
  }

  // swordEntity->GetComponent<Isetta::NetworkTransform>()->SetNetworkedParentToRoot();
  swordEntity->transform->SetParent(nullptr);
  targetX = randomX;
  isSwordFlying = true;
  flyDuration = 0;
  originY = transform->GetWorldPos().y;
  totalFlyDuration = (randomX - currentX) / v0x;
  v0y = (targetY - originY +
         0.5 * gravity * totalFlyDuration * totalFlyDuration) /
        totalFlyDuration;
  LOG_INFO(Isetta::Debug::Channel::General, "v0x = %f", v0x);

  // swordEntity->transform->SetWorldPos(
  //    Isetta::Math::Vector3{randomX, -0.25, 0});
  // swordEntity->transform->SetWorldRot(Isetta::Math::Vector3{0, 0, 0});
}
