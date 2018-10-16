/*
 * Copyright (c) 2018 Isetta
 */
#include "InEngineTestLevel.h"
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Core/Math/Vector3.h"
#include "FlyController.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Scene/Entity.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void InEngineTestLevel::LoadLevel() {
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
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));
}
}  // namespace Isetta