/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/CollisionSolverLevel/CollisionSolverLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/DebugCollision.h"
#include "Custom/KeyTransform.h"
#include "Custom/OscillateMove.h"
#include "Custom/RaycastClick.h"

#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Core/Math/Vector3.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Scene/Entity.h"

#include "Application.h"
#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/Collider.h"
#include "Collisions/CollisionHandler.h"
#include "Collisions/SphereCollider.h"
#include "Components/Editor/FrameReporter.h"
#include "Core/IsettaCore.h"

namespace Isetta {

void CollisionSolverLevel::OnLevelLoad() {
  Entity *debugEntity{Entity::CreateEntity("Debug")};
  debugEntity->AddComponent<FrameReporter>();

  // Camera
  Entity *cameraEntity = Entity::CreateEntity("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 10, 6}, Math::Vector3{-45, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>(false);

  // Light
  Entity *lightEntity{Entity::CreateEntity("Light")};
  lightEntity->AddComponent<LightComponent>("materials/light.material.xml",
                                            "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightEntity->AddComponent<EditorComponent>();

  Entity *grid{Entity::CreateEntity("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<RaycastClick>();

  // STATIC
  const int COLLIDERS = 3;
  Entity *staticCol[COLLIDERS];

  staticCol[0] = Entity::CreateEntity("box-collider", nullptr, true);
  staticCol[0]->SetTransform(Math::Vector3{0, 1, 0}, Math::Vector3{0, 0, 0});
  BoxCollider *bCol = staticCol[0]->AddComponent<BoxCollider>();
  bCol->mass = 100;
  CollisionHandler *handler = staticCol[0]->AddComponent<CollisionHandler>();
  handler->RegisterOnEnter([](Collider *const col) {
    LOG("collided with " + col->entity->GetName());
  });
  staticCol[0]->AddComponent<DebugCollision>();

  staticCol[1] = Entity::CreateEntity("sphere-collider", nullptr, true);
  staticCol[1]->SetTransform(Math::Vector3{0, 1, -4});
  SphereCollider *sCol = staticCol[1]->AddComponent<SphereCollider>();
  Collider *c = staticCol[1]->GetComponent<Collider>();
  c->mass = 1;

  staticCol[2] = Entity::CreateEntity("capsule-collider", nullptr, true);
  staticCol[2]->SetTransform(Math::Vector3{0, 1, -8});
  CapsuleCollider *cCol = staticCol[2]->AddComponent<CapsuleCollider>(
      false, Math::Vector3::zero, 0.5, 2, CapsuleCollider::Direction::X_AXIS);
  // staticCol[2]->AddComponent<DebugCollision>();
  cCol->mass = 50;

  //// DYNAMIC
  static Entity *box{Entity::CreateEntity("box-collider-dynamic")};
  box->SetTransform(Math::Vector3{3, 1, 0}, Math::Vector3{0, 0, 0});
  // box->transform->SetLocalRot(-45 * Math::Vector3::up);
  box->AddComponent<BoxCollider>();
  box->AddComponent<KeyTransform>();

  static Entity *sphere{Entity::CreateEntity("sphere-collider-dynamic")};
  sphere->SetTransform(Math::Vector3{3, 1, -4});
  sphere->AddComponent<SphereCollider>();
  sphere->AddComponent<KeyTransform>()->SetActive(false);

  static Entity *capsule{Entity::CreateEntity("capsule-collider-dynamic")};
  capsule->transform->SetLocalPos(Math::Vector3{3, 1, -8});
  capsule->transform->SetLocalRot(-30 * Math::Vector3::up);

  capsule->AddComponent<CapsuleCollider>(
      0.5, 2, static_cast<CapsuleCollider::Direction>(0));
  capsule->AddComponent<KeyTransform>()->SetActive(false);

  Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                  []() { Application::Exit(); });

  Input::RegisterKeyPressCallback(KeyCode::NUM1, [&]() {
    box->GetComponent<KeyTransform>()->SetActive(true);
    sphere->GetComponent<KeyTransform>()->SetActive(false);
    capsule->GetComponent<KeyTransform>()->SetActive(false);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM2, [&]() {
    box->GetComponent<KeyTransform>()->SetActive(false);
    sphere->GetComponent<KeyTransform>()->SetActive(true);
    capsule->GetComponent<KeyTransform>()->SetActive(false);
  });
  Input::RegisterKeyPressCallback(KeyCode::NUM3, [&]() {
    box->GetComponent<KeyTransform>()->SetActive(false);
    sphere->GetComponent<KeyTransform>()->SetActive(false);
    capsule->GetComponent<KeyTransform>()->SetActive(true);
  });

  Input::RegisterKeyPressCallback(
      KeyCode::NUM9, [&]() { sphere->GetComponent<Collider>()->mass -= 10; });
  Input::RegisterKeyPressCallback(
      KeyCode::NUM0, [&]() { sphere->GetComponent<Collider>()->mass += 10; });
}
}  // namespace Isetta
