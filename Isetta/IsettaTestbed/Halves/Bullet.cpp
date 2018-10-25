/*
 * Copyright (c) 2018 Isetta
 */
#include "Bullet.h"
#include "Custom/IsettaCore.h"
#include "Graphics/MeshComponent.h"
#include "GameManager.h"
#include "Zombie.h"

namespace Isetta {
class MeshComponent;

float Bullet::flySpeed = 50;

void Bullet::Initialize(const Math::Vector3& pos, const Math::Vector3& flyDir) {
  dir = flyDir.Normalized();
  GetTransform().SetWorldPos(pos);
  GetTransform().LookAt(GetTransform().GetWorldPos() + dir);
}

void Bullet::OnEnable() {
  if (!initialized) {
    entity->AddComponent<MeshComponent, true>("Bullet/Bullet.scene.xml");
    initialized = true;
    audio.SetAudioClip("bullet-impact.wav");
  }
  elapsedTime = 0.f;
}

void Bullet::Update() {
  if (!entity->GetActive()) return;

  GetTransform().TranslateWorld(dir * Time::GetDeltaTime() * flySpeed);
  elapsedTime += Time::GetDeltaTime();
  if (elapsedTime > lifeTime) {
    // TODO(YIDI): Add this when Destroy is working
    entity->SetActive(false);
  }
  for (const auto& zombie : GameManager::zombies) {
    if (!zombie->GetActive()) continue;
    float disSqrd = (zombie->GetTransform().GetWorldPos() + 1.5 * Math::Vector3::up - GetTransform().GetWorldPos()).SqrMagnitude();
    if (disSqrd < 1.f) {
      zombie->GetComponent<Zombie>()->TakeDamage(damage);
      entity->SetActive(false);
      LOG_INFO(Debug::Channel::Gameplay, "Zombie %s hit!", zombie->GetName().c_str());
      audio.Play(false, 1.0f);
    }
  }
}

}  // namespace Isetta
