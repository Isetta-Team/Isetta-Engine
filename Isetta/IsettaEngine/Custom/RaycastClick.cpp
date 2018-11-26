/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/RaycastClick.h"
#include "Core/Debug/DebugDraw.h"
#include "Input/Input.h"
#include "Scene/Transform.h"

#include "Collisions/Collider.h"
#include "Collisions/Collisions.h"
#include "Collisions/RaycastHit.h"
#include "Core/Debug/Logger.h"
#include "Core/Geometry/Ray.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"

namespace Isetta {
RaycastClick::RaycastClick(bool raycastAll, float maxDistance)
    : useRaycastAll{raycastAll}, maxRayDistance{maxDistance} {}

void RaycastClick::OnEnable() {
  Input::RegisterMousePressCallback(MouseButtonCode::MOUSE_LEFT, [&]() {
    Ray r =
        CameraComponent::Main()->ScreenPointToRay(Input::GetMousePosition());
    DebugDraw::Line(r.GetOrigin(),
                    r.GetPoint(maxRayDistance <= 0 ? 100.0f : maxRayDistance),
                    Color::red, 1.5f, 5);
    DebugDraw::Cube(CameraComponent::Main()->transform->GetLocalToWorldMatrix(),
                    Color::red, 5);

    if (useRaycastAll) {
      Array<RaycastHit> hits = Collisions::RaycastAll(r, maxRayDistance);
      LOG_INFO(Debug::Channel::Collisions, "Raycast Hits:");
      if (hits.Size() == 0) {
        LOG_INFO(Debug::Channel::Collisions, "~~~ None");
      } else {
        for (auto& hit : hits) {
          LOG_INFO(Debug::Channel::Collisions, "--- %s",
                   hit.GetCollider()->entity->GetName().c_str());
        }
      }
    } else {
      RaycastHit hitInfo;
      if (Collisions::Raycast(r, &hitInfo, maxRayDistance)) {
        DebugDraw::Point(hitInfo.GetPoint(), Color::red, 5, 5);
        LOG_INFO(Debug::Channel::Collisions, "Raycast Hit: %s",
                 hitInfo.GetCollider()->entity->GetName().c_str());
      } else {
        DebugDraw::Point(r.GetPoint(20), Color::brown, 5, 5);
      }
    }
  });
}
}  // namespace Isetta
