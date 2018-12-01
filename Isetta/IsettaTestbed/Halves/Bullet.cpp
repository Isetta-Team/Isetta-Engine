/*
 * Copyright (c) 2018 Isetta
 */
#include "Bullet.h"
#include "GameManager.h"
#include "Zombie.h"

namespace Isetta {
class MeshComponent;

float Bullet::flySpeed = 50;

// Bullet is pooled by player. Reactive is called when a player get a bullet
// from pool
void Bullet::Reactivate(const Math::Vector3& pos, const Math::Vector3& flyDir) {
  dir = flyDir.Normalized();
  transform->SetWorldPos(pos);
  transform->LookAt(transform->GetWorldPos() + dir);
}

void Bullet::OnEnable() {
  if (!initialized) {
    // initialize bullet for the first time
    entity->AddComponent<MeshComponent>("Halves/Bullet/Bullet.scene.xml");
    audio = entity->AddComponent<AudioSource>(
        AudioClip::Load("Halves/Sound/bullet-impact.wav"));
    initialized = true;
  }
  elapsedTime = 0.f;
}

void Bullet::Update() {
  if (!entity->GetActive()) return;

  transform->TranslateWorld(dir * Time::GetDeltaTime() * flySpeed);
  elapsedTime += Time::GetDeltaTime();
  // if the bullet been alive too long, destroy it for performance
  if (elapsedTime > lifeTime) {
    entity->SetActive(false);
  }
  // detect hitting zombie. This code is written before we had collisions
  for (const auto& zombie : GameManager::zombies) {
    if (!zombie->GetActive()) continue;
    float disSqrd = (zombie->transform->GetWorldPos() +
                     1.5 * Math::Vector3::up - transform->GetWorldPos())
                        .SqrMagnitude();
    if (disSqrd < 1.f) {
      zombie->GetComponent<Zombie>()->TakeDamage(damage);
      entity->SetActive(false);
      audio->Play();
    }
  }
}

}  // namespace Isetta
