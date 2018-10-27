/*
 * Copyright (c) 2018 Isetta
 */
#include "CameraController.h"
#include "Custom/IsettaCore.h"
#include "PlayerController.h"

namespace Isetta {

void CameraController::Update() {
  auto player = PlayerController::Instance();
  if (player != nullptr) {
    GetTransform()->SetWorldPos(player->GetTransform()->GetWorldPos() + Math::Vector3{0, y, z});
    Math::Vector3 dir = GetTransform()->GetWorldPos() - player->GetTransform()->GetWorldPos();
    GetTransform()->LookAt(GetTransform()->GetWorldPos() + dir);
  }
}

void CameraController::GuiUpdate() {
  float base = 150;
  float padding = 20;
  GUI::SliderFloat(RectTransform{Math::Rect{-200, base, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Camera Height", &y, 0, 30.f);
  GUI::SliderFloat(RectTransform{Math::Rect{-200, base + padding, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Camera HoriDis", &z, 0, 30.f);
}
}  // namespace Isetta
