/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"
#include "ExampleLevel.h"
#include "Custom/FlyController.h"
#include "Custom/ExampleComponent.h"

namespace Isetta {

using CameraProperty = CameraComponent::Property;

void ExampleLevel::LoadLevel() {
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

  Entity* customEntity{AddEntity("custom")};
  customEntity->AddComponent<ExampleComponent>();
}
}  // namespace Isetta