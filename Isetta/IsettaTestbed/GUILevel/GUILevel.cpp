/*
 * Copyright (c) 2018 Isetta
 */
#include "GUILevel.h"
#include "FontExample.h"
#include "GUIComponent.h"

using namespace Isetta;

void GUILevel::Load() {
  // Load font at filepath at size(s)
  // Font::AddFontFromFile("Fonts\\CONSOLA.TTF", 13.0f, "Consola");

  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<CameraComponent>();

  Entity* guiEntity = Entity::Instantiate("GUI");
  // Display some of GUI functionality
  guiEntity->AddComponent<GUIComponent>();
  // Display use of font in GUI
  guiEntity->AddComponent<FontExample>(16.f, "Halves/ZOMBIE.TTF");
}
