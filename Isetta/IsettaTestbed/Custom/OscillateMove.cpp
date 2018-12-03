/*
 * Copyright (c) 2018 Isetta
 */
#include "OscillateMove.h"

namespace Isetta {
// Check for valid axis of direction of movement (0-2)
void OscillateMove::OnEnable() {
  if (axis < 0 || axis > Math::Vector3::ELEMENT_COUNT - 1) {
    this->SetActive(false);
  }
}

// Move the transform each update
void OscillateMove::Update() {
  float dist = Math::Vector3::Dot(transform->GetLocalPos() - origin,
                                  transform->GetAxis(axis));
  if (distance < dist) {
    direction = -1;
  } else if (-distance > dist) {
    direction = 1;
  }
  transform->TranslateLocal(direction * transform->GetAxis(axis) * speed *
                            EngineLoop::GetGameClock().GetDeltaTime());
  // DebugDraw::Cube(Math::Matrix4::Translate(transform->GetWorldPos()));
}
}  // namespace Isetta