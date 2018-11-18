/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EmptyLevel/EmptyLevel.h"

#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Custom/EscapeExit.h"

namespace Isetta {
using CameraProperty = CameraComponent::Property;

void EmptyLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  // Application::Exit();
  cameraEntity->AddComponent<EscapeExit>();
}
}  // namespace Isetta