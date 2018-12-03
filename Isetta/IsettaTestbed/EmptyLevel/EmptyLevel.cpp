/*
 * Copyright (c) 2018 Isetta
 */
#include "EmptyLevel.h"
#include "Custom/EscapeExit.h"

namespace Isetta {

void EmptyLevel::Load() {
  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  // Quits the application
  // Application::Exit();

  // Escape Key exits application
  cameraEntity->AddComponent<EscapeExit>();
}
}  // namespace Isetta