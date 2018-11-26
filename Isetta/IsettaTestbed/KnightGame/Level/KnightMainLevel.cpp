/*
 * Copyright (c) 2018 Isetta
 */
#include "KnightMainLevel.h"

#include "Audio/AudioClip.h"
#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
#include "Components/GridComponent.h"
#include "Core/Math/Random.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Graphics/MeshComponent.h"
#include "Graphics/ParticleSystemComponent.h"
#include "Scene/Entity.h"
#include "Scene/Layers.h"
#include "Scene/Primitive.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/CollisionHandler.h"
#include "Collisions/Collisions.h"
#include "Collisions/SphereCollider.h"

#include "Components/AxisDrawer.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"

#include "KnightGame/Constants.h"
#include "KnightGame/Gameplay/Enemy.h"
#include "KnightGame/Gameplay/FireballCircle.h"
#include "KnightGame/Gameplay/FollowComponent.h"
#include "KnightGame/Gameplay/KnightController.h"
#include "KnightGame/Gameplay/ScreenShifter.h"
#include "KnightGame/Gameplay/SpinAttack.h"
#include "KnightGame/Gameplay/SwordController.h"

using namespace Isetta;

namespace KnightGame {
using LightProperty = LightComponent::Property;

void KnightMainLevel::OnLevelLoad() {
  Logger::channelMask.set(static_cast<int>(Debug::Channel::Memory), false);
  knightLayer = Layers::NewLayer(KNIGHT_LAYER);
  Collisions::SetIgnoreLayerCollision(knightLayer, knightLayer);

  Entity* cameraEntity = Entity::Instantiate("Camera");
  CameraComponent* camComp = cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<AudioListener>();
  cameraEntity->AddComponent<ScreenShifter>();
  // cameraEntity->AddComponent<FlyController>();

  Entity* lightEntity = Entity::Instantiate("Light");
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* editor{Entity::Instantiate("Editor")};
  editor->AddComponent<GridComponent>();
  editor->AddComponent<EditorComponent>();

  Entity* leftWall = Entity::Instantiate("Left Wall");
  BoxCollider* leftWallBox = leftWall->AddComponent<BoxCollider>(
      Math::Vector3{-8, 1.5f, 0}, Math::Vector3{1, 3, 10});
  leftWallBox->mass = 100000;
  CollisionHandler* handler = leftWall->AddComponent<CollisionHandler>();
  leftWall->AddComponent<ScreenShifter>();
  handler->RegisterOnEnter(
      [](Collider* const collider) { Entity::Destroy(collider->entity); });

  Entity* knight = Entity::Instantiate("Knight");
  knight->SetLayer(knightLayer);
  MeshComponent* mesh =
      knight->AddComponent<MeshComponent>(KNIGHT_PATH + "idle.scene.xml");
  AnimationComponent* animation =
      knight->AddComponent<AnimationComponent>(mesh);
  knight->AddComponent<CapsuleCollider>(Math::Vector3{0, 0.9f, 0.2f});
  knight->AddComponent<KnightController>();
  SpinAttack* spin = knight->AddComponent<SpinAttack>();

  // bool follow[3] = {true, false, false};
  // cameraEntity->AddComponent<FollowComponent>(knight, follow);

  Entity* sword = Entity::Instantiate("Sword");
  sword->SetLayer(knightLayer);
  sword->transform->SetParent(knight->transform);
  sword->AddComponent<MeshComponent>(KNIGHT_PATH + "sword_aligned.scene.xml");
  sword->AddComponent<SwordController>(mesh);

  Entity* swordCol = Entity::Instantiate("Sword Collider");
  swordCol->SetLayer(knightLayer);
  swordCol->transform->SetParent(sword->transform);
  swordCol->transform->SetLocalRot(Math::Vector3{0, 0, 90.f});
  swordCol->transform->SetLocalPos(-Math::Vector3::left);
  swordCol->AddComponent<AxisDrawer>();
  BoxCollider* swordBox = swordCol->AddComponent<BoxCollider>(
      Math::Vector3::zero, Math::Vector3{0.2f, 2.3f, 0.2f});
  swordBox->isTrigger = true;

  Entity* fireball = Entity::Instantiate("Fireball");
  fireball->AddComponent<ParticleSystemComponent, false>(
      "KnightGame/Fireball/particleSys1.scene.xml");
  FireballCircle* fireCircle =
      fireball->AddComponent<FireballCircle, false>(knight->transform, spin);

  AudioClip* const swordClip =
      AudioClip::Load("KnightGame\\Audio\\sword.aiff", "sword");
  for (int i = 0; i < 10; i++) {
    Entity* enemyEntity = Primitive::Create(Primitive::Type::Capsule, true);
    enemyEntity->SetTransform(Math::Vector3{
        5.f + i * 3.f, 1.f, (2.f * Math::Random::GetRandom01() - 1.f) * 5.f});
    enemyEntity->AddComponent<AudioSource>(0b001, swordClip);
    CollisionHandler* handler = enemyEntity->AddComponent<CollisionHandler>();
    Enemy* enemy = enemyEntity->AddComponent<Enemy>(handler, swordClip);
    fireCircle->enemies.push_back(enemy);
  }
}
}  // namespace KnightGame
