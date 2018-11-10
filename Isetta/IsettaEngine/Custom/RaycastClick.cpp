/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/RaycastClick.h"
#include "Core/Debug/DebugDraw.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

#include "Collisions/Collider.h"
#include "Collisions/Collisions.h"
#include "Collisions/Ray.h"
#include "Core/Debug/Logger.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

namespace Isetta {

void RaycastClick::OnEnable() {
  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, []() {
    Ray r =
        CameraComponent::Main()->ScreenPointToRay(Input::GetMousePosition());
    RaycastHit hitInfo;
    DebugDraw::Line(r.GetOrigin(), r.GetPoint(100.0f), Color::red, 1.5f, 5);
    DebugDraw::Cube(
        CameraComponent::Main()->GetTransform()->GetLocalToWorldMatrix(),
        Color::red, 5);
    if (Collisions::Raycast(r, &hitInfo)) {
      DebugDraw::Point(hitInfo.GetPoint(), Color::red, 5, 5);
      LOG_INFO(Debug::Channel::Collisions, "Raycast Hit: %s",
               hitInfo.GetCollider()->GetEntity()->GetName());
    } else {
      DebugDraw::Point(r.GetPoint(20), Color::brown, 5, 5);
    }
  });
}
}  // namespace Isetta
