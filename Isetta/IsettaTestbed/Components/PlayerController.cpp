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
  GUI::InputInt(RectTransform{Math::Rect{-200, 500, 300, 100},
                              GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                "Player Speed", &moveSpeedInt);
  moveSpeed = moveSpeedInt;

  GUI::InputInt(RectTransform{Math::Rect{-200, 550, 300, 100},
                              GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                "Player Scale", &scaleInt);
  GetTransform().SetLocalScale(scaleInt / 100.f * Math::Vector3::one);
}

PlayerController* PlayerController::Instance() { return instance; }

}  // namespace Isetta
