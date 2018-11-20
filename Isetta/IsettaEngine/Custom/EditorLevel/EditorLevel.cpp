/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EditorLevel/EditorLevel.h"

#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"

#include "Components/Editor/Console.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/Editor/Hierarchy.h"
#include "Components/Editor/Inspector.h"
#include "Custom/EscapeExit.h"

namespace Isetta {
using CameraProperty = CameraComponent::Property;

void EditorLevel::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  Entity* editor{AddEntity("Editor")};
  editor->AddComponent<EscapeExit>();
  editor->AddComponent<EditorComponent>();
  // editor->AddComponent<Console>("Console", true);
  // Inspector* inspector = editor->AddComponent<Inspector>("Inspector", false);
  // editor->AddComponent<Hierarchy>("Hierarchy", true, inspector);
  editor->AddComponent<EscapeExit>();
}
}  // namespace Isetta