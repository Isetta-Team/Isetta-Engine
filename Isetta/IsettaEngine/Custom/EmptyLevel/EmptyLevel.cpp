/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EmptyLevel/EmptyLevel.h"

#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Components/Editor/Console.h"
#include "Custom/EscapeExit.h"
#include "Custom/LoadNextLevel.h"

namespace Isetta {
using CameraProperty = CameraComponent::Property;

void EmptyLevel::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  // Application::Exit();
  cameraEntity->AddComponent<EscapeExit>();
  cameraEntity->AddComponent<LoadNextLevel>("EditorLevel");
}
}  // namespace Isetta