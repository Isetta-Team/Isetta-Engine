/*
 * Copyright (c) 2018 Isetta
 */
#include "Zombie.h"
#include "GameManager.h"
#include "PlayerController.h"

namespace Isetta {
float Zombie::speed = 10.f;

void Zombie::OnEnable() {
  if (!isInitialized) {
    // add mesh and animation component when first time initialized
    MeshComponent* mesh =
        entity->AddComponent<MeshComponent>("Halves/Zombie/Zombie.scene.xml");
    AnimationComponent* animation =
        entity->AddComponent<AnimationComponent>(mesh);
    animation->AddAnimation("Halves/Zombie/Zombie.anim", 0, "", false);
    // initialize audio too
    audio = entity->AddComponent<AudioSource>(
        AudioClip::Load("Halves/Sound/zombie-death.mp3"));
    isInitialized = true;
  }
  entity->GetComponent<AnimationComponent>()->Play();
  health = 100.f;
}

void Zombie::Update() {
  auto player = PlayerController::Instance();
  if (player == nullptr) return;

  // run to player
  Math::Vector3 dir =
      player->transform->GetWorldPos() - transform->GetWorldPos();
  transform->TranslateWorld(dir.Normalized() * Time::GetDeltaTime() * speed);
  transform->LookAt(transform->GetWorldPos() + dir);
}

// get shot by player
void Zombie::TakeDamage(const float damage) {
  health -= damage;
  if (health <= 0) {
    audio->Play();
    GameManager::score += (Math::Random::GetRandom01() / 2 + 0.5f) * 10;
    entity->SetActive(false);
  }
}
}  // namespace Isetta
