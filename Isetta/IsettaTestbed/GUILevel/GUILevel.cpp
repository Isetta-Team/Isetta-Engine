/*
 * Copyright (c) 2018 Isetta
 */
#include "GUILevel/GUILevel.h"

#include "Core/Config/Config.h"
#include "GUILevel/GUIComponent.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

using namespace Isetta;
using CameraProperty = CameraComponent::Property;

void GUILevel::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");

  cameraEntity->AddComponent<GUIComponent>();
}
