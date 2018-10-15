/*
 * Copyright (c) 2018 Isetta
 */
#include "PlayerController.h"
#include "Bullet.h"
#include "Custom/IsettaCore.h"
#include "Graphics/MeshComponent.h"

namespace Isetta {
PlayerController* PlayerController::instance;

void PlayerController::OnEnable() {
  instance = this;
  shootAudio.SetAudioClip("gunshot.aiff");

  Input::RegisterKeyPressCallback(KeyCode::T, [&]() { Shoot(); });
}

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
    cooldown -= dt;
    if (cooldown <= 0.f) {
      Shoot();
      cooldown = shootInterval;
    }
  } else {
    cooldown = 0.f;
  }
}

void PlayerController::GuiUpdate() {
  float base = 50;
  float interval = 20;
  GUI::SliderFloat(RectTransform{Math::Rect{-200, base, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Player Speed", &moveSpeed, 0, 30);

  GUI::SliderFloat(RectTransform{Math::Rect{-200, base + interval, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Player Scale", &scale, 0, 0.1f, 1);
  GetTransform().SetLocalScale(scale * Math::Vector3::one);

  GUI::SliderFloat(RectTransform{Math::Rect{-200, base + interval * 2, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Shoot Interval", &shootInterval, 0, .5f, 1);

  GUI::SliderFloat(RectTransform{Math::Rect{-200, base + interval * 3, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Bullet Speed", &Bullet::flySpeed, 0, 100.f, 1);
}

PlayerController* PlayerController::Instance() { return instance; }

void PlayerController::Shoot() {
  shootAudio.Play(false, 1);
  static int count = 0;
  count++;
  Entity* bullet{ADD_ENTITY(Util::StrFormat("Bullet (%d)", count))};
  auto bulletComp = bullet->AddComponent<Bullet>();
  bulletComp->Initialize(GetTransform().GetWorldPos() +
                             GetTransform().GetForward() * 0.7 -
                             GetTransform().GetLeft() * 0.1,
                         GetTransform().GetForward());
  // bulletComp->Initialize(GetTransform().GetWorldPos(),
  // GetTransform().GetForward());
}

}  // namespace Isetta
