/*
 * Copyright (c) 2018 Isetta
 */
#include "Bullet.h"
#include "Custom/IsettaCore.h"
#include "Graphics/MeshComponent.h"

namespace Isetta {
class MeshComponent;

float Bullet::flySpeed = 50;

void Bullet::Initialize(const Math::Vector3& pos, const Math::Vector3& flyDir) {
  dir = flyDir.Normalized();
  GetTransform().SetWorldPos(pos);
  GetTransform().LookAt(GetTransform().GetWorldPos() + dir);
}

void Bullet::OnEnable() {
  owner->AddComponent<MeshComponent>(true, "Bullet/Bullet.scene.xml");
}

void Bullet::Update() {
  GetTransform().TranslateWorld(dir * Time::GetDeltaTime() * flySpeed);
  elapsedTime += Time::GetDeltaTime();
  if (elapsedTime > lifeTime) {
    // Entity::Destroy(owner);
  }
}

}  // namespace Isetta
