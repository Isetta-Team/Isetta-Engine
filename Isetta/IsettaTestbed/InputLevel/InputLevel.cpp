/*
 * Copyright (c) 2018 Isetta
 */
#include "InputLevel/InputLevel.h"
#include "Graphics/CameraComponent.h"
#include "Core/Config/Config.h"
#include "Scene/Entity.h"
#include "InputTestComponent.h"

using namespace Isetta;
using CameraProperty = CameraComponent::Property;

void InputLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));
  cameraEntity->AddComponent<InputTestComponent>();
}
