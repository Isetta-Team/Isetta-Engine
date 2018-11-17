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
#include "Custom/IsettaCore.h"

namespace Isetta {
using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void CollisionSolverLevel::LoadLevel() {
  Entity *debugEntity{AddEntity("Debug")};
  debugEntity->AddComponent<FrameReporter>();

  // Camera
  Entity *cameraEntity{AddEntity("Camera")};
  CameraComponent *camComp =
      cameraEntity->AddComponent<CameraComponent>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 10, 6}, Math::Vector3{-45, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>(false);
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  // Light
  Entity *lightEntity{AddEntity("Light")};
  LightComponent *lightComp = lightEntity->AddComponent<LightComponent>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);
  lightEntity->AddComponent<EditorComponent>();

  Entity *grid{AddEntity("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<RaycastClick>();

  // STATIC
  const int COLLIDERS = 3;
  Entity *staticCol[COLLIDERS];

  staticCol[0] = AddEntity("box-collider", true);
  staticCol[0]->SetTransform(Math::Vector3{0, 1, 0}, Math::Vector3{0, 0, 0});
  BoxCollider *bCol = staticCol[0]->AddComponent<BoxCollider>();
  CollisionHandler *handler = staticCol[0]->AddComponent<CollisionHandler>();
  handler->RegisterOnEnter([](Collider *const col) {
    LOG("collided with " + col->GetEntity()->GetName());
  });
  staticCol[0]->AddComponent<DebugCollision>();

  staticCol[1] = AddEntity("sphere-collider", true);
  staticCol[1]->SetTransform(Math::Vector3{0, 1, -4});
  SphereCollider *sCol = staticCol[1]->AddComponent<SphereCollider>();
  Collider *c = staticCol[1]->GetComponent<Collider>();

  staticCol[2] = AddEntity("capsule-collider", true);
  staticCol[2]->SetTransform(Math::Vector3{0, 1, -8});
  CapsuleCollider *cCol = staticCol[2]->AddComponent<CapsuleCollider>(
      false, Math::Vector3::zero, 0.5, 2, CapsuleCollider::Direction::X_AXIS);
  // staticCol[2]->AddComponent<DebugCollision>();

  //// DYNAMIC
  static Entity *box{AddEntity("box-collider-dynamic")};
  box->SetTransform(Math::Vector3{3, 1, 0}, Math::Vector3{0, 0, 0});
  // box->GetTransform()->SetLocalRot(-45 * Math::Vector3::up);
  box->AddComponent<BoxCollider>();
  box->AddComponent<KeyTransform>();

  static Entity *sphere{AddEntity("sphere-collider-dynamic")};
  sphere->SetTransform(Math::Vector3{3, 1, -4});
  sphere->AddComponent<SphereCollider>();
  sphere->AddComponent<KeyTransform>()->SetActive(false);

  static Entity* capsule{AddEntity("capsule-collider-dynamic")};
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
}
}  // namespace Isetta
