/*
 * Copyright (c) 2018 Isetta
 */
#include "LevelLoadingLevel.h"

#include "Core/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Custom/EscapeExit.h"
#include "Custom/LevelLoadingMenu.h"
#include "LoadNextLevel.h"

namespace Isetta {
void LevelLoadingLevel::OnLevelLoad() {
  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<EscapeExit>();

  // Level Menu entity, brings up a menu to browse other levels
  Entity* levelMenu{Entity::Instantiate("Level Menu")};
  levelMenu->AddComponent<LevelLoadingMenu>();
  levelMenu->AddComponent<LoadNextLevel>("EditorLevel");
}
}  // namespace Isetta