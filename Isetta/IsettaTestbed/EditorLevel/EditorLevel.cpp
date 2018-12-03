/*
 * Copyright (c) 2018 Isetta
 */
#include "EditorLevel.h"
#include "Components/Editor/EditorComponent.h"
#include "Custom/EscapeExit.h"

namespace Isetta {

void EditorLevel::Load() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<EscapeExit>();

  // Entity holding editor components
  Entity* editor{Entity::Instantiate("Editor")};
  // EditorComponent has console, inspector, hierarchy, and frame reporter
  editor->AddComponent<EditorComponent>();

  // Each component can also be added individually
  // editor->AddComponent<Console>("Console", true);
  // Inspector* inspector = editor->AddComponent<Inspector>("Inspector", false);
  // editor->AddComponent<Hierarchy>("Hierarchy", true, inspector);
}
}  // namespace Isetta