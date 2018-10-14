/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/ExampleComponent.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Graphics/AnimationComponent.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {

void ExampleComponent::OnEnable() {
  Input::RegisterKeyPressCallback(KeyCode::F1, [&]() {
    Entity* man =
        LevelManager::Instance().currentLevel->GetEntityByName("PushAnim");
    if (man->GetTransform().GetParent() == &GetTransform()) {
      man->GetTransform().SetParent(nullptr);
      LOG_INFO(Debug::Channel::General, "Parent of %s is set to %s",
               man->GetName().c_str(), "null");
    } else {
      man->GetTransform().SetParent(&GetTransform());
      LOG_INFO(Debug::Channel::General, "Parent of %s is set to %s",
               man->GetName().c_str(), owner->GetName().c_str());
    }
  });
}

void ExampleComponent::Update() {
#if _DEBUG
  Entity* entity =
      LevelManager::Instance().currentLevel->GetEntityByName("PushAnim");

  if (entity != nullptr) {
    if (Input::IsKeyPressed(KeyCode::N)) {
      entity->GetComponent<MeshComponent>()->SetActive(false);
    }
    if (Input::IsKeyPressed(KeyCode::M)) {
      entity->GetComponent<AnimationComponent>()->Play();
    }
  }

  DebugDraw::WireCube(GetTransform().GetLocalToWorldMatrix());

  float speed = 3.f;
  float rotSpeed = 30.;
  float dt = EngineLoop::GetGameClock().GetDeltaTime();

  if (Input::IsKeyPressed(KeyCode::UP)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * speed);
  }

  if (Input::IsKeyPressed(KeyCode::DOWN)) {
    GetTransform().TranslateWorld(GetTransform().GetForward() * dt * -speed);
  }

  if (Input::IsKeyPressed(KeyCode::LEFT)) {
    GetTransform().RotateLocal(GetTransform().GetUp(), rotSpeed * dt);
  }

  if (Input::IsKeyPressed(KeyCode::RIGHT)) {
    GetTransform().RotateLocal(GetTransform().GetUp(), -rotSpeed * dt);
  }
#endif
}
}  // namespace Isetta
