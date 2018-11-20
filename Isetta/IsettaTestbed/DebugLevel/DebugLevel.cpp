/*
 * Copyright (c) 2018 Isetta
 */
#include "DebugLevel/DebugLevel.h"

#include "Components/FlyController.h"
#include "Core/Config/Config.h"
#include "DebugLevel/DebugComponent.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

using namespace Isetta;
void DebugLevel::OnLevelLoad() {
  Entity* cameraEntity{CREATE_ENTITY("Camera")};
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<DebugComponent>();
}
