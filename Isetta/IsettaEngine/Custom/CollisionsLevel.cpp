/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/CollisionsLevel.h"

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
#include "IsettaCore.h"
#include "FrameReporter.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;
using ColliderAttribute = Collider::Property;

void CollisionsLevel::LoadLevel() {
  Entity* debugEntity{AddEntity("Debug")};
  debugEntity->AddComponent<FrameReporter>();

  // Camera
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  // Light
  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);

  Entity* grid{AddEntity("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<RaycastClick>();

  // STATIC
  const int COLLIDERS = 3;
  Entity* staticCol[COLLIDERS];

  staticCol[0] = AddEntity("box-collider");
  staticCol[0]->SetTransform(Math::Vector3{0, 1, 0}, Math::Vector3{0, 0, 0});
  BoxCollider* bCol = staticCol[0]->AddComponent<BoxCollider>();
  bCol->SetProperties(ColliderAttribute::IS_STATIC, true);
  CollisionHandler* handler = staticCol[0]->AddComponent<CollisionHandler>();
  handler->RegisterOnEnter([](Collider* const col) {
    LOG("collided with " + col->GetEntity()->GetName());
  });
  staticCol[0]->AddComponent<DebugCollision>();

  staticCol[1] = AddEntity("sphere-collider");
  staticCol[1]->SetTransform(Math::Vector3{0, 1, -4});
  SphereCollider* sCol = staticCol[1]->AddComponent<SphereCollider>();
  Collider* c = staticCol[1]->GetComponent<Collider>();
  sCol->SetProperties(ColliderAttribute::IS_STATIC, true);

  staticCol[2] = AddEntity("capsule-collider");
  staticCol[2]->SetTransform(Math::Vector3{0, 1, -8});
  CapsuleCollider* cCol = staticCol[2]->AddComponent<CapsuleCollider>(
      true, false, Math::Vector3::zero, 0.5, 2,
      CapsuleCollider::Direction::X_AXIS);
  // staticCol[2]->AddComponent<DebugCollision>();

  //// DYNAMIC
  for (int i = 0; i < COLLIDERS; i++) {
    Entity* oscillator{AddEntity("oscillator")};
    oscillator->transform->SetParent(staticCol[i]->transform);
    oscillator->transform->SetLocalPos(7 * Math::Vector3::left);
    oscillator->AddComponent<OscillateMove>(0, 1, -1, 12);
    oscillator->AddComponent<KeyTransform>(0.25);
    oscillator->AddComponent<CollisionHandler>();

    Entity* box{AddEntity("box-collider" + i)};
    box->transform->SetParent(oscillator->transform);
    box->transform->SetLocalPos(-2 * Math::Vector3::left);
    box->AddComponent<BoxCollider>();

    Entity* sphere{AddEntity("sphere-collider" + i)};
    sphere->transform->SetParent(oscillator->transform);
    sphere->transform->SetLocalPos(Math::Vector3::zero);
    sphere->AddComponent<SphereCollider>();

    for (int j = 0; j < 3; j++) {
      Entity* capsule{AddEntity("capsule-collider" + i + j)};
      capsule->transform->SetParent(oscillator->transform);
      capsule->transform->SetLocalPos(3 * (j + 1) * Math::Vector3::left);
      capsule->transform->SetLocalRot(-30 * Math::Vector3::up);
      CapsuleCollider* col = capsule->AddComponent<CapsuleCollider>(
          0.5, 2, static_cast<CapsuleCollider::Direction>(j));
    }

    Input::RegisterKeyPressCallback(KeyCode::ESCAPE,
                                    []() { Application::Exit(); });
  }
}
}  // namespace Isetta
