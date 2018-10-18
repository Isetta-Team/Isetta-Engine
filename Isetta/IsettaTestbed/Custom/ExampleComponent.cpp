/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/ExampleComponent.h"
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
  Input::RegisterKeyPressCallback(KeyCode::NUM1, [&]() {
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
  Entity* entity =
      LevelManager::Instance().currentLevel->GetEntityByName("PushAnim");

  if (entity == nullptr) return;
  if (Input::IsKeyPressed(KeyCode::N)) {
    entity->GetComponent<MeshComponent>()->SetActive(false);
  }
  if (Input::IsKeyPressed(KeyCode::M)) {
    entity->GetComponent<AnimationComponent>()->Play();
  }

#if _DEBUG
  float speed = 10.f;

  if (Input::IsKeyPressed(KeyCode::LEFT_ARROW)) {
    GetTransform().TranslateWorld(Math::Vector3::left *
                                  EngineLoop::GetGameClock().GetDeltaTime() *
                                  speed);
    GetTransform().Print();
  }

  if (Input::IsKeyPressed(KeyCode::RIGHT_ARROW)) {
    GetTransform().TranslateWorld(Math::Vector3::right *
                                  EngineLoop::GetGameClock().GetDeltaTime() *
                                  speed);
  }

  if (Input::IsKeyPressed(KeyCode::P)) {
    GetTransform().Print();
  }
#endif
}
}  // namespace Isetta
