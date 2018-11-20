/*
 * Copyright (c) 2018 Isetta
 */
#include "InputLevel/InputLevel.h"
#include "Core/Config/Config.h"
#include "Graphics/CameraComponent.h"
#include "InputTestComponent.h"
#include "Scene/Entity.h"

using namespace Isetta;

void InputLevel::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->AddComponent<InputTestComponent>();
}
