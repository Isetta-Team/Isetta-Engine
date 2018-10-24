/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/RaycastClick.h"
#include "Core/Debug/DebugDraw.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

#include "Collisions/Collisions.h"
#include "Collisions/Ray.h"
#include "Core/Debug/Logger.h"
#include "Graphics/CameraComponent.h"

namespace Isetta {

void RaycastClick::OnEnable() {
  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, []() {
    LOG("CLICK");
    Ray r =
        CameraComponent::Main()->ScreenPointToRay(Input::GetMousePosition());
    RaycastHit hitInfo;
    DebugDraw::Line(r.GetOrigin(), r.GetDirection() * 100.f, Color::red, 1.5f,
                    10);
    if (Collisions::Raycast(r, &hitInfo)) {
      // DebugDraw::Line(hitInfo.GetPoint(), hitInfo.GetNormal(), Color::blue,
      //                1.5f, 30);
      DebugDraw::Point(hitInfo.GetPoint(), Color::red, 15, 30);
    }
  });
}
}  // namespace Isetta
