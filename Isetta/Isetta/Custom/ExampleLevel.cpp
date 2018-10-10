/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/ExampleLevel.h"
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Core/Math/Vector3.h"
#include "ExampleComponent.h"
#include "FlyController.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Graphics/MeshComponent.h"
#include "Scene/Entity.h"
#include "AxisDrawer.h"
#include "PlayerController.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void ExampleLevel::LoadLevel() {
  Entity* man{AddEntity("PushAnim")};
  man->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 0, 0});
  man->AddComponent<AxisDrawer>();
  man->AddComponent<PlayerController>();
  MeshComponent* pushMesh =
      man->AddComponent<MeshComponent>(true, "push/Pushing.scene.xml");

  AnimationComponent* ani =
      man->AddComponent<AnimationComponent>(true, pushMesh);
  ani->AddAnimation("push/Pushing.anim", 0, "", false);

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>(
      true, "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);

  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent>(true, "Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();
  camComp->SetProperty<CameraProperty::FOV>(
      Config::Instance().renderConfig.fieldOfView.GetVal());
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      Config::Instance().renderConfig.nearClippingPlane.GetVal());
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      Config::Instance().renderConfig.farClippingPlane.GetVal());

  Entity* customEntity{AddEntity("custom")};
  customEntity->AddComponent<ExampleComponent>();
  customEntity->AddComponent<AxisDrawer>();
}
}  // namespace Isetta