/*
 * Copyright (c) 2018 Isetta
 */
#include "DebugLevel/DebugLevel.h"

#include "Core/Config/Config.h"
#include "DebugLevel/DebugComponent.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

using namespace Isetta;
using CameraProperty = CameraComponent::Property;

void DebugLevel::LoadLevel() {
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

  cameraEntity->AddComponent<DebugComponent>();
}
