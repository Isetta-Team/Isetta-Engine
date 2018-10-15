/*
 * Copyright (c) 2018 Isetta
 */
#include "Zombie.h"
#include "Custom/IsettaCore.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/AnimationComponent.h"
#include "PlayerController.h"
#include "GameManager.h"
#include "Core/Math/Random.h"

namespace Isetta {
float Zombie::speed = 10.f;

void Zombie::OnEnable() {
  if (!isInitialized) {

  MeshComponent* mesh =
      owner->AddComponent<MeshComponent>(true, "Zombie/Zombie.scene.xml");
  AnimationComponent* animation =
      owner->AddComponent<AnimationComponent>(true, mesh);
  animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
    audio.SetAudioClip("zombie-death.mp3");
    isInitialized = true;
  }
  owner->GetComponent<AnimationComponent>()->Play();
  health = 100.f;
}

void Zombie::Update() {
  auto player = PlayerController::Instance();
  if (player == nullptr) return;

  Math::Vector3 dir = player->GetTransform().GetWorldPos() - GetTransform().GetWorldPos();
  GetTransform().TranslateWorld(dir.Normalized() * Time::GetDeltaTime() * speed);
  GetTransform().LookAt(GetTransform().GetWorldPos() + dir);
}

void Zombie::TakeDamage(float damage) {
  health -= damage;
  if (health <= 0) {
    audio.Play(false, 1.0f);
    GameManager::score += (Math::Random::GetRandom01() / 2 + 0.5f) * 10;
    owner->SetActive(false);
  }
}
}  // namespace Isetta
