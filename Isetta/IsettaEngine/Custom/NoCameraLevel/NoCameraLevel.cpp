/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/NoCameraLevel/NoCameraLevel.h"

#include "Core/Config/Config.h"
#include "Core/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Custom/NoCameraLevel/NoCameraComponent.h"

namespace Isetta {
void NoCameraLevel::Load() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<NoCameraComponent>();
}
}  // namespace Isetta