/*
 * Copyright (c) 2018 Isetta
 */
#include "RaycastClick.h"

namespace Isetta {
RaycastClick::RaycastClick(bool raycastAll, float maxDistance)
    : useRaycastAll{raycastAll}, maxRayDistance{maxDistance} {}

// Register left mouse click to raycast and display line
void RaycastClick::OnEnable() {
  Input::RegisterMousePressCallback(MouseButton::LEFT, [this]() {
    Ray r =
        CameraComponent::Main()->ScreenPointToRay(Input::GetMousePosition());
    DebugDraw::Line(r.GetOrigin(),
                    r.GetPoint(maxRayDistance <= 0 ? 100.0f : maxRayDistance),
                    Color::red, 1.5f, 5);
    DebugDraw::Cube(CameraComponent::Main()->transform->GetLocalToWorldMatrix(),
                    Color::red, 5);

    if (useRaycastAll)
      RaycastAll(r);
    else
      Raycast(r);
  });
}

// Raycast colliders for closest hit, diplaying the closest hit
void RaycastClick::Raycast(const Ray& ray) {
  RaycastHit hitInfo;
  if (Collisions::Raycast(ray, &hitInfo, maxRayDistance)) {
    DebugDraw::Point(hitInfo.GetPoint(), Color::red, 5, 5);
    LOG_INFO(Debug::Channel::Collisions, "Raycast Hit: %s",
             hitInfo.GetCollider()->entity->GetName().c_str());
  } else {
    LOG_INFO(Debug::Channel::Collisions, "~~~ None");
  }
}

// Raycast all colliders, diplaying each hit point
void RaycastClick::RaycastAll(const Ray& ray) {
  Array<RaycastHit> hits = Collisions::RaycastAll(ray, maxRayDistance);
  LOG_INFO(Debug::Channel::Collisions, "Raycast Hits:");
  if (hits.Size() == 0) {
    LOG_INFO(Debug::Channel::Collisions, "~~~ None");
  } else {
    for (auto& hit : hits) {
      DebugDraw::Point(hit.GetPoint(), Color::red, 5, 5);
      LOG_INFO(Debug::Channel::Collisions, "--- %s",
               hit.GetCollider()->entity->GetName().c_str());
    }
  }
}
}  // namespace Isetta
