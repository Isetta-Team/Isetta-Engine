/*
 * Copyright (c) 2018 Isetta
 */
#include "Zombie.h"
#include "Core/Math/Random.h"
#include "Custom/IsettaCore.h"
#include "GameManager.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/MeshComponent.h"
#include "PlayerController.h"

namespace Isetta {
float Zombie::speed = 10.f;

void Zombie::OnEnable() {
  if (!isInitialized) {
    MeshComponent* mesh =
        entity->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml");
    AnimationComponent* animation =
        entity->AddComponent<AnimationComponent>(mesh);
    animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
    audio = entity->AddComponent<AudioSource>(AudioClip::LoadClip("Sound\\zombie-death.mp3"));
    isInitialized = true;
  }
  entity->GetComponent<AnimationComponent>()->Play();
  health = 100.f;
}

void Zombie::Update() {
  auto player = PlayerController::Instance();
  if (player == nullptr) return;

  Math::Vector3 dir =
      player->transform->GetWorldPos() - transform->GetWorldPos();
  transform->TranslateWorld(dir.Normalized() * Time::GetDeltaTime() * speed);
  transform->LookAt(transform->GetWorldPos() + dir);
}

void Zombie::TakeDamage(const float damage) {
  health -= damage;
  if (health <= 0) {
    audio->Play();
    GameManager::score += (Math::Random::GetRandom01() / 2 + 0.5f) * 10;
    entity->SetActive(false);
  }
}
}  // namespace Isetta
