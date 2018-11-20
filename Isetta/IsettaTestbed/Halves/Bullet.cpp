/*
 * Copyright (c) 2018 Isetta
 */
#include "Bullet.h"
#include "Custom/IsettaCore.h"
#include "GameManager.h"
#include "Graphics/MeshComponent.h"
#include "Zombie.h"

namespace Isetta {
class MeshComponent;

float Bullet::flySpeed = 50;

void Bullet::Reactivate(const Math::Vector3& pos, const Math::Vector3& flyDir) {
  dir = flyDir.Normalized();
  transform->SetWorldPos(pos);
  transform->LookAt(transform->GetWorldPos() + dir);
}

void Bullet::OnEnable() {
  if (!initialized) {
    entity->AddComponent<MeshComponent, true>("Bullet/Bullet.scene.xml");
    initialized = true;
    audio = entity->AddComponent<AudioSource>("Sound/bullet-impact.wav");
  }
  elapsedTime = 0.f;
}

void Bullet::Update() {
  if (!entity->GetActive()) return;

  transform->TranslateWorld(dir * Time::GetDeltaTime() * flySpeed);
  elapsedTime += Time::GetDeltaTime();
  if (elapsedTime > lifeTime) {
    // TODO(YIDI): Add this when Destroy is working
    entity->SetActive(false);
  }
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
