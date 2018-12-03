/*
 * Copyright (c) 2018 Isetta
 */
#include "FireballCircle.h"

#include "Enemy.h"
#include "SpinAttack.h"

using namespace Isetta;

namespace KnightGame {
void FireballCircle::Start() {
  particleSys = entity->GetComponent<ParticleSystemComponent>();
  spin->SetCallback([&]() {
    SetActive(true);
    spin->SetActive(false);
  });
  AudioClip* const flamesClip =
      AudioClip::Load("KnightGame\\Audio\\flames.aiff", "flames");
  src = entity->AddComponent<AudioSource>(0b011, flamesClip);
}

void FireballCircle::OnEnable() {
  angle = 0;
  Math::Vector3 position =
      Math::Vector3{radius, 1.f, 0} + center->GetWorldPos();
  transform->SetWorldPos(position);
  if (particleSys) particleSys->SetActive(true);
  if (src) src->Play();
}

void FireballCircle::OnDisable() {
  if (particleSys) particleSys->SetActive(false);
  spin->SetActive(true);
  if (src && src->IsPlaying()) src->Stop();
}

void FireballCircle::Update() {
  elapsedTime += Time::GetDeltaTime();
  angle = speed * elapsedTime;
  Math::Vector3 position = Math::Vector3{radius * Math::Util::Cos(angle), 1.f,
                                         radius * Math::Util::Sin(angle)} +
                           center->GetWorldPos();
  transform->SetWorldPos(position);

  const float sqRadius = 0.05f * radius * radius;
  Math::Vector2 xyPos =
      Math::Vector2{transform->GetWorldPos().x, transform->GetWorldPos().z};
  auto it = enemies.begin();
  while (it != enemies.end()) {
    Math::Vector2 enemyPos = Math::Vector2{(*it)->transform->GetWorldPos().x,
                                           (*it)->transform->GetWorldPos().z};
    if ((enemyPos - xyPos).SqrMagnitude() < sqRadius) {
      (*it)->Explode();
      it = enemies.erase(it);
    } else
      ++it;
  }

  if (elapsedTime > timeUp) {
    elapsedTime = 0;
    SetActive(false);
  }
}

}  // namespace KnightGame