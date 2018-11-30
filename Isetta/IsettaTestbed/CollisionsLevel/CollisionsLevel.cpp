/*
 * Copyright (c) 2018 Isetta
 */
#include "CollisionsLevel.h"

#include "Components/FlyController.h"
#include "Components/GridComponent.h"

#include "Components/Editor/FrameReporter.h"

#include "Custom/DebugCollision.h"
#include "Custom/EscapeExit.h"
#include "Custom/KeyTransform.h"
#include "Custom/OscillateMove.h"
#include "Custom/RaycastClick.h"

namespace Isetta {

void CollisionsLevel::Load() {
  // Camera
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<EscapeExit>();

  // Light
  Entity* lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  // Grid
  Entity* grid = Entity::Instantiate("Grid");
  grid->AddComponent<GridComponent>();

  // Debug
  Entity* debugEntity{Entity::Instantiate("Debug")};
  debugEntity->AddComponent<FrameReporter>();
  debugEntity->AddComponent<RaycastClick>(true, 5);

  /// Static Entities
  const int COLLIDERS = 3;
  Entity* staticCol[COLLIDERS];

  // Static, Trigger BoxCollider
  staticCol[0] = Entity::Instantiate("box-collider", nullptr, true);
  staticCol[0]->SetTransform(Math::Vector3{0, 1, 0}, Math::Vector3{0, 0, 0});
  BoxCollider* bCol = staticCol[0]->AddComponent<BoxCollider>();
  bCol->isTrigger = true;
  // CollisionHandler has callbacks of Enter/Stay/Exit
  CollisionHandler* handler = staticCol[0]->AddComponent<CollisionHandler>();
  // CollisionHandler registering OnEnter callback
  handler->RegisterOnEnter([](Collider* const col) {
    LOG("collided with " + col->entity->GetName());
  });
  // DebugCollision changes the color of the collider when colliding
  staticCol[0]->AddComponent<DebugCollision>();

  // Static, Trigger SphereCollider
  staticCol[1] = Entity::Instantiate("sphere-collider", nullptr, true);
  staticCol[1]->SetTransform(Math::Vector3{0, 1, -4});
  SphereCollider* sCol = staticCol[1]->AddComponent<SphereCollider>();
  sCol->isTrigger = true;
  Collider* c = staticCol[1]->GetComponent<Collider>();
  staticCol[1]->AddComponent<CollisionHandler>();
  staticCol[1]->AddComponent<DebugCollision>();

  // Static, Trigger CapsuleCollider
  staticCol[2] = Entity::Instantiate("capsule-collider", nullptr, true);
  staticCol[2]->SetTransform(Math::Vector3{0, 1, -8});
  CapsuleCollider* cCol = staticCol[2]->AddComponent<CapsuleCollider>(
      true, Math::Vector3::zero, 0.5, 2, CapsuleCollider::Direction::X_AXIS);
  staticCol[2]->AddComponent<CollisionHandler>();
  staticCol[2]->AddComponent<DebugCollision>();

  /// Dynamic Entities
  for (int i = 0; i < COLLIDERS; i++) {
    // Oscillator Entity shifts left/right
    Entity* oscillator{Entity::Instantiate("oscillator")};
    oscillator->transform->SetParent(staticCol[i]->transform);
    oscillator->transform->SetLocalPos(7 * Math::Vector3::left);
    // OscillateMove controls the position of the entity
    // oscillator->AddComponent<OscillateMove>(0, 1, -1, 12);
    // KeyTransform allows the user to move the entity with keys
    oscillator->AddComponent<KeyTransform>(0.25);
    oscillator->AddComponent<CollisionHandler>();

    // Non-static, Trigger BoxCollider
    Entity* box{Entity::Instantiate("box-collider" + i)};
    box->transform->SetParent(oscillator->transform);
    box->transform->SetLocalPos(-2 * Math::Vector3::left);
    c = box->AddComponent<BoxCollider>();
    c->isTrigger = true;
    box->AddComponent<CollisionHandler>();
    box->AddComponent<DebugCollision>();

    // Non-static, Trigger SphereCollider
    Entity* sphere{Entity::Instantiate("sphere-collider" + i)};
    sphere->transform->SetParent(oscillator->transform);
    sphere->transform->SetLocalPos(Math::Vector3::zero);
    c = sphere->AddComponent<SphereCollider>();
    c->isTrigger = true;
    sphere->AddComponent<CollisionHandler>();
    sphere->AddComponent<DebugCollision>();

    // Non-static, Trigger CapsuleColliders each on a different axis and rotated
    for (int j = 0; j < 3; j++) {
      Entity* capsule{Entity::Instantiate("capsule-collider" + i + j)};
      capsule->transform->SetParent(oscillator->transform);
      capsule->transform->SetLocalPos(3 * (j + 1) * Math::Vector3::left);
      capsule->transform->SetLocalRot(-30 * Math::Vector3::up);
      CapsuleCollider* col = capsule->AddComponent<CapsuleCollider>(
          0.5, 2, static_cast<CapsuleCollider::Direction>(j));
      col->isTrigger = true;
      capsule->AddComponent<CollisionHandler>();
      capsule->AddComponent<DebugCollision>();
    }
  }
}
}  // namespace Isetta
