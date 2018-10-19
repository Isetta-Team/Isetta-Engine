/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/CollisionsLevel.h"

#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/KeyTransform.h"
#include "Custom/OscillateMove.h"
#include "Custom/RaycastClick.h"

#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Core/Math/Vector3.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Scene/Entity.h"

#include "Collisions/BoxCollider.h"
#include "Collisions/CapsuleCollider.h"
#include "Collisions/Collider.h"
#include "Collisions/SphereCollider.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;
using ColliderAttribute = Collider::Attributes;

void CollisionsLevel::LoadLevel() {
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
  Entity* staticCol[3];

  staticCol[0] = AddEntity("collider");
  staticCol[0]->SetTransform(Math::Vector3{0, 1, 0});
  BoxCollider* bCol = staticCol[0]->AddComponent<BoxCollider>();
  bCol->SetAttribute(ColliderAttribute::IS_STATIC, true);

  staticCol[1] = AddEntity("collider");
  staticCol[1]->SetTransform(Math::Vector3{0, 1, -4});
  SphereCollider* sCol = staticCol[1]->AddComponent<SphereCollider>();
  sCol->SetAttribute(ColliderAttribute::IS_STATIC, true);

  staticCol[2] = AddEntity("collider");
  staticCol[2]->SetTransform(Math::Vector3{0, 1, -8});
  CapsuleCollider* cCol = staticCol[2]->AddComponent<CapsuleCollider>(
      true, false, Math::Vector3::zero, 0.5, 2,
      CapsuleCollider::Direction::X_AXIS);

  //// DYNAMIC
  /*
  for (int i = 0; i < 3; i++) {
    Entity* oscillator{AddEntity("oscillator")};
    oscillator->GetTransform().SetParent(&staticCol[i]->GetTransform());
    oscillator->GetTransform().SetLocalPos(7 * Math::Vector3::left);
    // oscillator->GetTransform().SetLocalPos(7 * Math::Vector3::left +
    //                                       Math::Vector3::up);
    oscillator->AddComponent<OscillateMove>(0, 2, -1, 12);
    // oscillator->AddComponent<KeyTransform>(0.25);

    Entity* box{AddEntity("collider")};
    box->GetTransform().SetParent(&oscillator->GetTransform());
    box->GetTransform().SetLocalPos(-2 * Math::Vector3::left);
    box->AddComponent<BoxCollider>();

    Entity* sphere{AddEntity("collider")};
    sphere->GetTransform().SetParent(&oscillator->GetTransform());
    sphere->GetTransform().SetLocalPos(Math::Vector3::zero);
    sphere->AddComponent<SphereCollider>();

    for (int j = 0; j < 3; j++) {
      Entity* capsule{AddEntity("collider")};
      capsule->GetTransform().SetParent(&oscillator->GetTransform());
      capsule->GetTransform().SetLocalPos(3 * (j + 1) * Math::Vector3::left);
      CapsuleCollider* col = capsule->AddComponent<CapsuleCollider>(
          0.5, 2, static_cast<CapsuleCollider::Direction>(j));
    }
  }
  */
}
}  // namespace Isetta
