/*
 * Copyright (c) 2018 Isetta
 */
#include "KnightMainLevel.h"
#include "Components/AxisDrawer.h"
#include "Components/Editor/EditorComponent.h"
#include "KnightGame/Constants.h"
#include "KnightGame/Gameplay/Enemy.h"
#include "KnightGame/Gameplay/FireballCircle.h"
#include "KnightGame/Gameplay/FollowComponent.h"
#include "KnightGame/Gameplay/KnightController.h"
#include "KnightGame/Gameplay/ScoreManager.h"
#include "KnightGame/Gameplay/ScreenShifter.h"
#include "KnightGame/Gameplay/SpinAttack.h"
#include "KnightGame/Gameplay/SwordController.h"

using namespace Isetta;

namespace KnightGame {
using LightProperty = LightComponent::Property;

void KnightMainLevel::Load() {
  // Turn off Logger messages for Memory (works for other channels)
  Logger::channelMask.set(static_cast<int>(Debug::Channel::Memory), false);
  // Create a layer with name, store int
  knightLayer = Layers::NewLayer(KNIGHT_LAYER);
  // Ignore collisions between knightLayer collisions
  Collisions::SetIgnoreLayerCollision(knightLayer, knightLayer);

  Entity* cameraEntity = Entity::Instantiate("Camera");
  CameraComponent* camComp = cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<AudioListener>();
  // Custom component for shifting to screen right
  cameraEntity->AddComponent<ScreenShifter>();
  // cameraEntity->AddComponent<FlyController>();

  Entity* lightEntity = Entity::Instantiate("Light");
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  // Debug Components
  // Entity* editor{Entity::Instantiate("Editor")};
  // editor->AddComponent<GridComponent>();
  // editor->AddComponent<EditorComponent>();

  // Destructor Wall
  Entity* leftWall = Entity::Instantiate("Left Wall");
  // BoxCollider(center, size)
  BoxCollider* leftWallBox = leftWall->AddComponent<BoxCollider>(
      Math::Vector3{-8, 1.5f, 0}, Math::Vector3{1, 3, 10});
  // Mass is moveable
  leftWallBox->mass = 100000;
  // Handler is callback holder
  CollisionHandler* handler = leftWall->AddComponent<CollisionHandler>();
  leftWall->AddComponent<ScreenShifter>();
  // Handler callback collider OnEnter
  handler->RegisterOnEnter([](Collider* const collider) {
    // Check for KnightController component
    if (collider->entity->GetComponent<KnightController>()) {
      // Destroy entity
      Entity::Destroy(collider->entity);
      // Create Event
      EventObject eventObject{
          GAMEOVER_EVENT, Time::GetFrameCount(), EventPriority::HIGH, {}};
      // RaiseEvent
      Isetta::Events::Instance().RaiseImmediateEvent(eventObject);
    } else {
      Enemy* enemy;
      enemy = collider->entity->GetComponent<Enemy>();
      if (enemy)
        enemy->Reset();
      else {
        collider->entity->transform->SetLocalPos(Math::Vector3::zero);
        enemy = collider->entity->GetComponentInParent<Enemy>();
        if (enemy) enemy->Reset();
      }
    }
  });

  // Score Manager entity
  Entity* score = Entity::Instantiate("Score");
  score->AddComponent<ScoreManager>();

  // Knight Entity (mesh + animation)
  Entity* knight = Entity::Instantiate("Knight");
  // Set layer (used for collision & comparison)
  knight->SetLayer(knightLayer);
  MeshComponent* knightMesh =
      knight->AddComponent<MeshComponent>(KNIGHT_PATH + "idle.scene.xml");
  AnimationComponent* animation =
      knight->AddComponent<AnimationComponent>(knightMesh);
  knight->AddComponent<CapsuleCollider>(Math::Vector3{0, 0.9f, 0.2f});
  // Knight movement
  KnightController* knightController = knight->AddComponent<KnightController>();
  // Flame attack
  SpinAttack* spin = knight->AddComponent<SpinAttack>();

  // Camera follow entity on true axes (not used for game)
  // bool follow[3] = {true, false, false};
  // cameraEntity->AddComponent<FollowComponent>(knightMesh, follow);

  // Sword (mesh)
  Entity* sword = Entity::Instantiate("Sword");
  sword->SetLayer(knightLayer);
  // Set parent
  sword->transform->SetParent(knight->transform);
  sword->AddComponent<MeshComponent>(KNIGHT_PATH + "sword_aligned.scene.xml");
  // Sword controller
  sword->AddComponent<SwordController>(knightMesh, knightController);

  Entity* swordCol = Entity::Instantiate("Sword Collider");
  swordCol->SetLayer(knightLayer);
  swordCol->transform->SetParent(sword->transform);
  // Set local rotation (relative to parent)
  swordCol->transform->SetLocalRot(Math::Vector3{0, 0, 90.f});
  // Set local position (relative to parent)
  swordCol->transform->SetLocalPos(-Math::Vector3::left);
  swordCol->AddComponent<AxisDrawer>();
  BoxCollider* swordBox = swordCol->AddComponent<BoxCollider>(
      Math::Vector3::zero, Math::Vector3{0.2f, 2.3f, 0.2f});
  swordBox->isTrigger = true;

  // Fireball (particle)
  Entity* fireball = Entity::Instantiate("Fireball");
  fireball->AddComponent<ParticleSystemComponent, false>();
  // Fireball action
  FireballCircle* fireCircle =
      fireball->AddComponent<FireballCircle, false>(knight->transform, spin);

  // Sword audio
  AudioClip* const swordClip =
      AudioClip::Load("KnightGame\\Audio\\sword.aiff", "sword");

  // Pool entities
  for (int i = 0; i < enemyPool; i++) {
    // Create capsule primitive with collider
    Entity* enemyEntity = Primitive::Create(Primitive::Type::Capsule, true);
    enemyEntity->SetTransform(Math::Vector3{
        3.f + i * 3.f, 1.f, (2.f * Math::Random::GetRandom01() - 1.f) * 5.f});
    // AudioSource: 3D positioned
    enemyEntity->AddComponent<AudioSource>(0b001, swordClip);
    CollisionHandler* handler = enemyEntity->AddComponent<CollisionHandler>();
    // Sets as entity
    Enemy* enemy = enemyEntity->AddComponent<Enemy>(handler, swordClip);
    fireCircle->enemies.push_back(enemy);
  }
}
}  // namespace KnightGame
