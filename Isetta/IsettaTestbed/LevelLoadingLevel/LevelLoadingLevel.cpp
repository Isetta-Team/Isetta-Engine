/*
 * Copyright (c) 2018 Isetta
 */
#include "LevelLoadingLevel.h"
#include "Custom/EscapeExit.h"
#include "Custom/LevelLoadingMenu.h"
#include "LoadNextLevel.h"

namespace Isetta {
void LevelLoadingLevel::Load() {
  Font::AddFontFromFile("Fonts\\CONSOLA.TTF", 13.0f, "Consola");

  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<EscapeExit>();

  Entity* levelMenu{Entity::Instantiate("Level Menu")};
  // LevelLoadingMenu brings up a menu to browse other levels
  levelMenu->AddComponent<LevelLoadingMenu>();
  // LoadNextLevel loads the level of specified string,
  //  throws exception if level doesn't exist
  levelMenu->AddComponent<LoadNextLevel>("EditorLevel");
}
}  // namespace Isetta