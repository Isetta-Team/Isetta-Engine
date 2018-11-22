/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EmptyLevel/EmptyLevel.h"

#include "Core/Config/Config.h"
#include "Core/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Custom/EscapeExit.h"

namespace Isetta {

void EmptyLevel::OnLevelLoad() {
  Entity* cameraEntity{Entity::CreateEntity("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  // Application::Exit();
  cameraEntity->AddComponent<EscapeExit>();
}
}  // namespace Isetta