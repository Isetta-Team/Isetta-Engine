/*
 * Copyright (c) 2018 Isetta
 */
#include "Enemy.h"

#include "Audio/AudioSource.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/CollisionHandler.h"
#include "Graphics/MeshComponent.h"
#include "Scene/Entity.h"
#include "Scene/Primitive.h"

#include "KnightGame/Constants.h"

namespace KnightGame {
void Enemy::OnEnable() {
  handle = handler->RegisterOnEnter(
      std::bind(&Enemy::OnCollisionEnter, this, std::placeholders::_1));
  src = entity->GetComponent<AudioSource>();
}

void Enemy::OnDisable() { handler->UnregisterOnEnter(handle); }

void Enemy::OnCollisionEnter(Collider* const collider) {
  Entity* newBlock = Primitive::Create(Primitive::Type::Sphere, false);
  newBlock->transform->SetWorldPos(transform->GetWorldPos());
  entity->GetComponent<CapsuleCollider>()->SetActive(false);
  entity->GetComponent<MeshComponent>()->SetActive(false);

  src->clip = swordClip;
  src->Play();
}

void Enemy::Explode() {
  Entity* newBlock = Primitive::Create(Primitive::Type::Sphere, false);
  newBlock->transform->SetWorldPos(transform->GetWorldPos());
  entity->GetComponent<CapsuleCollider>()->SetActive(false);
  entity->GetComponent<MeshComponent>()->SetActive(false);
}
}  // namespace KnightGame