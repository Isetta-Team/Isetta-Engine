/*
 * Copyright (c) 2018 Isetta
 */
#include "PlayerController.h"
#include "Custom/IsettaCore.h"
#include "imgui.h"

namespace Isetta {
PlayerController* PlayerController::instance;

void PlayerController::OnEnable() { instance = this; }

void PlayerController::Update() {
  float dt = Time::GetDeltaTime();
  Math::Vector3 dir;
  bool shouldMove = false;

  if (Input::IsKeyPressed(KeyCode::UP)) {
    dir += Math::Vector3::back;
    shouldMove = true;
  }

  if (Input::IsKeyPressed(KeyCode::DOWN)) {
    dir += Math::Vector3::forward;
    shouldMove = true;
  }

  if (Input::IsKeyPressed(KeyCode::LEFT)) {
    dir += Math::Vector3::right;
    shouldMove = true;
  }

  if (Input::IsKeyPressed(KeyCode::RIGHT)) {
    dir += Math::Vector3::left;
    shouldMove = true;
  }

  if (shouldMove && dir.Magnitude() >= 1.f) {
    dir.Normalize();
    GetTransform().TranslateWorld(dir * moveSpeed * dt);
    GetTransform().LookAt(GetTransform().GetWorldPos() +
                          Math::Vector3::Slerp(GetTransform().GetForward(), dir,
                                               lookSmooth * dt));
    DebugDraw::Line(GetTransform().GetWorldPos(),
                    GetTransform().GetWorldPos() + dir);
  }
}

void PlayerController::GuiUpdate() {
  GUI::SliderFloat(RectTransform{Math::Rect{-200, 50, 300, 100},
                              GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                "Player Speed", &moveSpeed, 0, 30);

  GUI::SliderFloat(RectTransform{Math::Rect{-200, 80, 300, 100},
                              GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                "Player Scale", &scale, 0, 0.1f, 1);
  GetTransform().SetLocalScale(scale * Math::Vector3::one);
}

PlayerController* PlayerController::Instance() { return instance; }

}  // namespace Isetta
