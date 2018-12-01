/*
 * Copyright (c) 2018 Isetta
 */
#include "PlayerController.h"
#include "Bullet.h"

namespace Isetta {
PlayerController* PlayerController::instance;

void PlayerController::OnEnable() {
  instance = this;

  // initialize gunshot audio
  if (shootAudio == nullptr) {
    shootAudio = entity->AddComponent<AudioSource>(
        AudioClip::Load("Halves/Sound/gunshot.aiff"));
  }

  shootAudio->SetVolume(1.f);
  shootAudio->SetProperty(AudioSource::Property::IS_3D, false);

  // create the bullet pool
  bullets.reserve(bulletPoolSize);
  for (int i = 0; i < bulletPoolSize; i++) {
    Entity* bullet{Entity::Instantiate(Util::StrFormat("Bullet (%d)", i))};
    bullet->AddComponent<Bullet>();
    bullet->SetActive(false);
    bullets.push_back(bullet);
  }
}

void PlayerController::Start() {
  animationComp = entity->GetComponent<AnimationComponent>();
}

void PlayerController::Update() {
  // Legacy code, we were testing Entity::Destroy function
  // But also an example of getting input from gamepad buttons
  if (Input::IsGamepadButtonPressed(GamepadButton::Y)) {
    auto light = LevelManager::Instance().loadedLevel->GetEntityByName("Light");
    if (light != nullptr) {
      Entity::Destroy(light);
    }
  }

  float dt = Time::GetDeltaTime();
  Math::Vector3 lookDir;
  Math::Vector3 movement{};

  // Get input from Gamepad joysticks
  movement +=
      Input::GetGamepadAxis(GamepadAxis::L_HORIZONTAL) * Math::Vector3::left +
      Input::GetGamepadAxis(GamepadAxis::L_VERTICAL) * Math::Vector3::forward;

  if (movement.Magnitude() > 1) {
    movement.Normalize();
  }

  if (movement.Magnitude() > 0) {
    if (!isMoving) {
      isMoving = true;
      animationComp->TransitToAnimationState(1, 0.2f);
    }
    transform->TranslateWorld(movement * moveSpeed * dt);
  } else {
    if (isMoving) {
      isMoving = false;
      animationComp->TransitToAnimationState(0, 0.2f);
    }
  }

  lookDir +=
      Input::GetGamepadAxis(GamepadAxis::R_HORIZONTAL) * Math::Vector3::left +
      Input::GetGamepadAxis(GamepadAxis::R_VERTICAL) * Math::Vector3::forward;

  if (lookDir.Magnitude() >= 1.f) {
    lookDir.Normalize();
    transform->LookAt(transform->GetWorldPos() + lookDir);
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
  // Draw tweakable values
  float base = 50;
  float interval = 20;
  GUI::SliderFloat(RectTransform{Math::Rect{-200, base, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Player Speed", &moveSpeed, 0, 30);

  GUI::SliderFloat(RectTransform{Math::Rect{-200, base + interval, 300, 100},
                                 GUI::Pivot::TopRight, GUI::Pivot::TopRight},
                   "Player Scale", &scale, 0, 0.1f, 1);
  transform->SetLocalScale(scale * Math::Vector3::one);

  GUI::SliderFloat(
      RectTransform{Math::Rect{-200, base + interval * 2, 300, 100},
                    GUI::Pivot::TopRight, GUI::Pivot::TopRight},
      "Shoot Interval", &shootInterval, 0, .5f, 1);

  GUI::SliderFloat(
      RectTransform{Math::Rect{-200, base + interval * 3, 300, 100},
                    GUI::Pivot::TopRight, GUI::Pivot::TopRight},
      "Bullet Speed", &Bullet::flySpeed, 0, 100.f, 1);
}

PlayerController* PlayerController::Instance() { return instance; }

void PlayerController::Shoot() {
  shootAudio->Play();
  Entity* bullet = nullptr;

  // Get a bullet from pool
  for (auto& bul : bullets) {
    if (!bul->GetActive()) {
      bullet = bul;
      break;
    }
  }

  // Reactivate bullet
  if (bullet != nullptr) {
    bullet->SetActive(true);
    bullet->GetComponent<Bullet>()->Reactivate(
        transform->GetWorldPos() + transform->GetForward() * 0.7 -
            transform->GetLeft() * 0.1 + transform->GetUp() * 1.5,
        transform->GetForward());
  }
}

}  // namespace Isetta
