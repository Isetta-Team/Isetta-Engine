/*
 * Copyright (c) 2018 Isetta
 */
#include "DebugLevel.h"
#include "Components/FlyController.h"
#include "DebugComponent.h"

using namespace Isetta;
void DebugLevel::Load() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->AddComponent<FlyController>();
  // See DebugComponent for more details
  cameraEntity->AddComponent<DebugComponent>();
}
