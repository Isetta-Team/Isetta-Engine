/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EditorLevel/EditorLevel.h"

#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"

#include "Components/Editor/EditorComponent.h"
#include "Custom/EscapeExit.h"

#include "Scene/Primitive.h"

namespace Isetta {

void EditorLevel::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
      cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  Entity* editor{AddEntity("Editor")};
  editor->AddComponent<EditorComponent>();
  // editor->AddComponent<Console>("Console", true);
  // Inspector* inspector = editor->AddComponent<Inspector>("Inspector", false);
  // editor->AddComponent<Hierarchy>("Hierarchy", true, inspector);
  editor->AddComponent<EscapeExit>();

  Primitive::Create(Primitive::Type::Cube);
  Primitive::Create(Primitive::Type::Cube);
}
}  // namespace Isetta