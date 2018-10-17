/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/AnimationComponent.h"
#include "ExampleComponent.h"
#include "Custom/IsettaCore.h"

namespace Isetta {

void ExampleComponent::OnEnable() {
  Input::RegisterKeyPressCallback(KeyCode::F1, [&]() {
    Entity* man = Entity::GetEntityByName("PushAnim");
    if (man->GetTransform().GetParent() == &GetTransform()) {
      man->GetTransform().SetParent(nullptr);
    } else {
      man->GetTransform().SetParent(&GetTransform());
    }
  });
}

void ExampleComponent::Update() {
  DebugDraw::WireCube(GetTransform().GetLocalToWorldMatrix());
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
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * speed);
  }

  if (Input::IsKeyPressed(KeyCode::DOWN_ARROW)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * -speed);
  }

  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    GetTransform().RotateLocal(GetTransform().GetUp(), rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    GetTransform().RotateLocal(GetTransform().GetUp(), -rotSpeed * dt);
  }
}
}  // namespace Isetta
