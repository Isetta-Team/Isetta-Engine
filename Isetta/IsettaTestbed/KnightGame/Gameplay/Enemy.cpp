/*
 * Copyright (c) 2018 Isetta
 */
#include "Enemy.h"

#include "KnightGame/Constants.h"
#include "KnightGame/Gameplay/ScoreManager.h"
#include "KnightGame/Gameplay/SwordController.h"

namespace KnightGame {
void Enemy::OnEnable() {
  handle = handler->RegisterOnEnter(
      std::bind(&Enemy::OnCollisionEnter, this, std::placeholders::_1));
  src = entity->GetComponent<AudioSource>();

  capsule = entity->GetComponent<CapsuleCollider>();
  mesh = entity->GetComponent<MeshComponent>();

  deadBody = Primitive::Create(Primitive::Type::Sphere, true);
  deadBody->SetLayer(KNIGHT_LAYER);
  deadBody->transform->SetParent(entity->transform);
  deadBody->transform->SetLocalPos(Math::Vector3::zero);
  deadBody->SetActive(false);
}

void Enemy::OnDisable() { handler->UnregisterOnEnter(handle); }

void Enemy::OnCollisionEnter(Collider* const collider) {
  if (collider->entity->GetComponentInParent<SwordController>()) {
    Explode();

    src->clip = swordClip;
    src->Play();
  }
}

void Enemy::Explode() {
  deadBody->SetActive(true);
  capsule->SetActive(false);
  mesh->SetActive(false);
  ScoreManager::Instance()->AddPoint();
}

void Enemy::Reset() {
  Math::Vector3 position = transform->GetWorldPos();
  position.x += 3.f * enemyPool;
  position.z = (2.f * Math::Random::GetRandom01() - 1.f) * 4.5f;
  transform->SetWorldPos(position);
  deadBody->SetActive(false);
  capsule->SetActive(true);
  mesh->SetActive(true);
}
}  // namespace KnightGame