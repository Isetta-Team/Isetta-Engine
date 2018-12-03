/*
 * Copyright (c) 2018 Isetta
 */
#include "EmptyLevelForNetworkLoadLevel.h"
#include "Custom/EscapeExit.h"
#include "NetworkLevel/NetworkTestComp.h"

namespace Isetta {

void EmptyLevelForNetworkLoadLevel::Load() {
  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  cameraEntity->AddComponent<EscapeExit>();

  Entity* networkTestEntity = Entity::Instantiate("Network Test");
  networkTestEntity->AddComponent<NetworkTestComp>();
}
}  // namespace Isetta