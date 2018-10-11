/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/ExampleLevel.h"
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Core/Math/Vector3.h"
#include "ExampleComponent.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Graphics/MeshComponent.h"
#include "Scene/Entity.h"

namespace Isetta {

void ExampleLevel::LoadLevel() {
  Entity* pushEntity{AddEntity("PushAnim")};
  MeshComponent* pushMesh =
      pushEntity->AddComponent<MeshComponent>(true, "push/Pushing.scene.xml");
  pushEntity->SetTransform(Math::Vector3{-200, -100, 0},
                           Math::Vector3{0, 90, 0}, Math::Vector3::one);
  AnimationComponent* ani =
      pushEntity->AddComponent<AnimationComponent>(true, pushMesh);
  ani->AddAnimation("push/Pushing.anim", 0, "", false);

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent>(
      true, "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);
  lightComp->SetLightProperties<LightComponent::LightProperties::LIGHT_RADIUS>(
      0, 2500);
  lightComp->SetLightProperties<LightComponent::LightProperties::FIELD_OF_VIEW>(
      0, 180);
  lightComp->SetLightProperties<LightComponent::LightProperties::COLOR>(
      Color::white);
  lightComp
      ->SetLightProperties<LightComponent::LightProperties::COLOR_MULTIPLIER>(
          0, 1.0f);
  lightComp
      ->SetLightProperties<LightComponent::LightProperties::SHADOW_MAP_COUNT>(
          1);
  lightComp
      ->SetLightProperties<LightComponent::LightProperties::SHADOW_MAP_BIAS>(
          0, 0.01f);

  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent>(true, "Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 50, 600}, Math::Vector3::zero,
                             Math::Vector3::one);
  camComp
      ->SetCameraProperties<CameraComponent::CameraProperties::FIELD_OF_VIEW>(
          Config::Instance().renderConfig.fieldOfView.GetVal());
  camComp->SetCameraProperties<
      CameraComponent::CameraProperties::NEAR_CLIPPING_PLANE>(
      Config::Instance().renderConfig.nearClippingPlane.GetVal());
  camComp->SetCameraProperties<
      CameraComponent::CameraProperties::FAR_CLIPPING_PLANE>(
      Config::Instance().renderConfig.farClippingPlane.GetVal());

  Entity* customEntity{AddEntity("custom")};
  ExampleComponent* customComponent =
      customEntity->AddComponent<ExampleComponent>(true);
}
}  // namespace Isetta