/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EditorLevel/EditorLevel.h"

#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Components/Editor/Console.h"
#include "Components/Editor/Editor.h"
#include "Components/Editor/Hierarchy.h"
#include "Components/Editor/Inspector.h"
#include "Custom/EscapeExit.h"

namespace Isetta {
using CameraProperty = CameraComponent::Property;

void EditorLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  cameraEntity->AddComponent<EscapeExit>();

  Entity* editor{AddEntity("Editor")};
  editor->AddComponent<Editor>();
  // editor->AddComponent<Console>("Console", true);
  // Inspector* inspector = editor->AddComponent<Inspector>("Inspector", false);
  // editor->AddComponent<Hierarchy>("Hierarchy", true, inspector);
}
}  // namespace Isetta