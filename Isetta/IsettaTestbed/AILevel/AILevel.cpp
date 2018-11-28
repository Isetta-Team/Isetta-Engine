/*
 * Copyright (c) 2018 Isetta
 */
#include "AILevel.h"

#include "AITestComponent.h"
#include "Components/Editor/EditorComponent.h"
#include "Core/IsettaCore.h"
#include "Custom/EscapeExit.h"
#include "Custom/KeyTransform.h"
#include "Graphics/ParticleSystemComponent.h"
#include "Scene/Primitive.h"

void Isetta::AILevel::Load() {
  Entity *camera = Entity::Instantiate("Camera");
  camera->SetTransform(Math::Vector3{5, 5, 16}, Math::Vector3{-20, 0, 0},
                       Math::Vector3::one);
  camera->AddComponent<CameraComponent>();
  Entity *lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  camera->AddComponent<EscapeExit>();
  // camera->AddComponent<EditorComponent>();

  Entity *moveCube{Primitive::Create(Primitive::Type::Cube)};
  moveCube->SetTransform(Math::Vector3{5, 0, 5}, Math::Vector3::zero,
                         Math::Vector3::one * 0.2);
  auto p = moveCube->AddComponent<ParticleSystemComponent>();
  Input::RegisterKeyPressCallback(KeyCode::L, [p]() { p->SetActive(false); });
  Input::RegisterKeyPressCallback(KeyCode::O, [p]() { p->SetActive(true); });

  moveCube->AddComponent<KeyTransform>();
  camera->AddComponent<AITestComponent>(
      Math::Rect{0, 0, 10, 10}, Math::Vector2Int{20, 20}, moveCube->transform);
}
