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

void InputLevel::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->AddComponent<InputTestComponent>();
}
