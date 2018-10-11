/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/ExampleComponent.h"
#include "Graphics/AnimationComponent.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"
#include "Scene/Entity.h"
#include "Scene/Level.h"
#include "Scene/LevelManager.h"

namespace Isetta {

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
}
}  // namespace Isetta
