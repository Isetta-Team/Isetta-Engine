/*
 * Copyright (c) 2018 Isetta
 */
#include "ExampleComponent.h"
#include "Custom/IsettaCore.h"
#include "Graphics/AnimationComponent.h"

namespace Isetta {

void ExampleComponent::OnEnable() {
  Input::RegisterKeyPressCallback(KeyCode::F1, [&]() {
    Entity* man = Entity::GetEntityByName("PushAnim");
    if (man->transform->GetParent() == transform) {
      man->transform->SetParent(nullptr);
    } else {
      man->transform->SetParent(transform);
    }
  });
}

void ExampleComponent::Update() {
  DebugDraw::WireCube(transform->GetLocalToWorldMatrix());
  Entity* entity = Entity::GetEntityByName("PushAnim");

  if (entity != nullptr) {
    if (Input::IsKeyPressed(KeyCode::N)) {
      entity->GetComponent<MeshComponent>()->SetActive(false);
    }
    if (Input::IsKeyPressed(KeyCode::M)) {
      entity->GetComponent<AnimationComponent>()->Play();
    }
  }

  float speed = 3.f;
  float rotSpeed = 30.;
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::UP_ARROW)) {
    transform->TranslateWorld(transform->GetForward() * dt * speed);
  }

  if (Input::IsKeyPressed(KeyCode::DOWN_ARROW)) {
    transform->TranslateWorld(transform->GetForward() * dt * -speed);
  }

  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    transform->RotateLocal(transform->GetUp(), rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    transform->RotateLocal(transform->GetUp(), -rotSpeed * dt);
  }
}
}  // namespace Isetta
