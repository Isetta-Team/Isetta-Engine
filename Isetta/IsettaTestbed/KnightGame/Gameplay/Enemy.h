/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

using namespace Isetta;

namespace Isetta {
class CapsuleCollider;
class Collider;
class CollisionHandler;

class AudioClip;
class AudioSource;

class Entity;
class MeshComponent;
}  // namespace Isetta

namespace KnightGame {
DEFINE_COMPONENT(Enemy, Component, true)
private:
CollisionHandler* handler;
U64 handle;
AudioSource* src;
AudioClip* const swordClip;
Entity* deadBody;
MeshComponent* mesh;
CapsuleCollider* capsule;

public:
Enemy() = default;
Enemy(CollisionHandler* handler, AudioClip* const swordClip)
    : handler{handler}, swordClip{swordClip} {}

void OnEnable() override;
void OnDisable() override;
void OnCollisionEnter(Collider* const collider);

void Explode();
void Reset();
};  // namespace KnightGame
}  // namespace KnightGame

using namespace KnightGame;
REGISTER_COMPONENT(Enemy, Component, true)