/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/NoCameraLevel/NoCameraLevel.h"

#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/Font.h"

#include "Custom/EscapeExit.h"
#include "NoCameraComponent.h"

namespace Isetta {
void NoCameraLevel::OnLevelLoad() {
  auto font =
      Font::AddFontFromFile("font\\ProggyClean.ttf", 30.f, "ProggyClean");
  // Font::AddDefaultFont(font);

  Entity* cameraEntity{Entity::CreateEntity("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<EscapeExit>();
  cameraEntity->AddComponent<NoCameraComponent>();
}
}  // namespace Isetta