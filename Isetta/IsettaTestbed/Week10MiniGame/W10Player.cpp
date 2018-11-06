/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/W10Player.h"
#include "Core/Math/Matrix3.h"
#include "Custom/IsettaCore.h"
#include "Events/Events.h"
#include "Networking/NetworkManager.h"
#include "W10NetworkManager.h"

W10Player::W10Player(bool isRight)
    : isOnRight{isRight},
      horizontalSpeed{0.85},
      swordEntity{nullptr},
      swordPos{0},
      swordTargetX{1.25},
      swordXProgress{0},
      stabSpeed{7},
      swordStabStatus{0},
      gravity(9.8),
      v0x{isRight ? -2.f : 2.f},
      targetY(-0.25),
      isSwordFlying(false) {}

void W10Player::Awake() {
  swordEntity = ADD_ENTITY("Sword");
  swordEntity->GetTransform()->SetParent(GetTransform());
  swordEntity->GetTransform()->SetLocalPos(
      Isetta::Math::Vector3((isOnRight ? 1 : -1) * 0.75f, 0, 0.5f));

  // swordEntity->GetTransform()->SetLocalPos(Isetta::Math::Vector3{0.75, 0,
  // 1});
  swordEntity->GetTransform()->SetLocalScale(
      Isetta::Math::Vector3{1.5, 0.1, 0.1});
  swordEntity->AddComponent<Isetta::MeshComponent>("primitive/cube.scene.xml");

  Isetta::Events::Instance().RegisterEventListener(
      "Blocked",
      [&](const Isetta::EventObject& eventObject) { SwordBlocked(); });
}

void W10Player::Start() {
  Isetta::Input::RegisterKeyPressCallback(
      Isetta::KeyCode::W, [&]() { ChangeSwordVerticlePosition(1); });
  Isetta::Input::RegisterKeyPressCallback(
      Isetta::KeyCode::S, [&]() { ChangeSwordVerticlePosition(-1); });
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::SPACE, [&]() {
    if (swordStabStatus == 0) swordStabStatus = 1;
  });
}

void W10Player::Update() {
  float direction{0};
  if (Isetta::Input::IsKeyPressed(Isetta::KeyCode::A)) {
    direction -= 1;
  }
  if (Isetta::Input::IsKeyPressed(Isetta::KeyCode::D)) {
    direction += 1;
  }
  GetTransform()->TranslateWorld(direction * horizontalSpeed *
                                 Isetta::Time::GetDeltaTime() *
                                 Isetta::Math::Vector3::left);

  ChangeSwordHorizontalPosition(Isetta::Time::GetDeltaTime());

  if (!isSwordFlying && swordStabStatus == 3) {
    if (Isetta::Math::Util::Abs(GetTransform()->GetWorldPos().x -
                                swordEntity->GetTransform()->GetWorldPos().x) <
        0.2f) {
      swordEntity->GetTransform()->SetParent(GetTransform());
      swordEntity->GetTransform()->SetLocalPos(
          Isetta::Math::Vector3((isOnRight ? 1 : -1) * 0.75f, 0, 0.5f));
      swordStabStatus = 0;
      swordPos = 0;
    }
  }

  if (isSwordFlying) {
    flyDuration += Isetta::Time::GetDeltaTime();
    if (flyDuration > totalFlyDuration) {
      swordEntity->GetTransform()->SetWorldPos(
          Isetta::Math::Vector3{targetX, targetY, 0});
      isSwordFlying = false;
      flyDuration = 0;
    } else {
      float y = originY + v0y * flyDuration -
                0.5 * gravity * flyDuration * flyDuration;
      float x = targetX - (totalFlyDuration - flyDuration) * v0x;
      swordEntity->GetTransform()->SetWorldPos(Isetta::Math::Vector3{x, y, 0});
    }
  }
}

void W10Player::ChangeSwordVerticlePosition(int direction) {
  if (swordStabStatus != 0) return;
  swordPos += direction;
  swordPos = Isetta::Math::Util::Clamp(-1, 1, swordPos);
  W10SwordPosMessage* swordMessage =
      Isetta::NetworkManager::Instance()
          .GenerateMessageFromClient<W10SwordPosMessage>();
  swordMessage->swordPos = swordPos;
  Isetta::NetworkManager::Instance().SendMessageFromClient(swordMessage);
  auto swordLocalPos = swordEntity->GetTransform()->GetLocalPos();
  swordLocalPos.y = swordPos * 0.3;
  swordEntity->GetTransform()->SetLocalPos(swordLocalPos);
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
  auto swordLocalPos = swordEntity->GetTransform()->GetLocalPos();
  swordLocalPos.x =
      Isetta::Math::Util::Lerp(0.75, swordTargetX, swordXProgress);
  swordEntity->GetTransform()->SetLocalPos(swordLocalPos);

  if (swordXProgress == 1) {
    // to revoke
    W10AttackAttemptMessage* attackMessage{
        Isetta::NetworkManager::Instance()
            .GenerateMessageFromClient<W10AttackAttemptMessage>()};
    Isetta::NetworkManager::Instance().SendMessageFromClient(attackMessage);
    swordStabStatus = 2;
  }
  if (swordXProgress == 0) {
    // to idle
    swordStabStatus = 0;
  }
}

void W10Player::SwordBlocked() {
  swordStabStatus = 3;
  float randomX;
  float currentX{GetTransform()->GetWorldPos().x};
  if (isOnRight) {
    randomX = -2 + Isetta::Math::Random::GetRandom01() * (currentX - -2);
  } else {
    randomX = 2 + Isetta::Math::Random::GetRandom01() * (currentX - 2);
  }

  swordEntity->GetTransform()->SetParent(nullptr);
  targetX = randomX;
  isSwordFlying = true;
  flyDuration = 0;
  originY = GetTransform()->GetWorldPos().y;
  totalFlyDuration = (randomX - currentX) / v0x;
  v0y = (targetY - originY +
         0.5 * gravity * totalFlyDuration * totalFlyDuration) /
        totalFlyDuration;
  LOG_INFO(Isetta::Debug::Channel::General, "v0x = %f", v0x);

  // swordEntity->GetTransform()->SetWorldPos(
  //    Isetta::Math::Vector3{randomX, -0.25, 0});
  // swordEntity->GetTransform()->SetWorldRot(Isetta::Math::Vector3{0, 0, 0});
}
