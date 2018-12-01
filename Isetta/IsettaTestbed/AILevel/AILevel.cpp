/*
 * Copyright (c) 2018 Isetta
 */
#include "AILevel.h"

#include "AITestComponent.h"
#include "Custom/EscapeExit.h"
#include "Custom/KeyTransform.h"

void Isetta::AILevel::Load() {
  Entity *camera = Entity::Instantiate("Camera");
  camera->SetTransform(Math::Vector3{5, 5, 16}, Math::Vector3{-20, 0, 0},
                       Math::Vector3::one);
  camera->AddComponent<CameraComponent>();
  camera->AddComponent<EscapeExit>();

  Entity *lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  // Create the cube you control
  Entity *moveCube{Primitive::Create(Primitive::Type::Cube)};
  moveCube->SetTransform(Math::Vector3{5, 0, 5}, Math::Vector3::zero,
                         Math::Vector3::one * 0.2);
  // Add the particle system component
  // You can leave the constructor empty if you just want to use the default
  // fire particle, otherwise, please include the particle xml
  auto p = moveCube->AddComponent<ParticleSystemComponent>();
  // There is only active or deactive for particles, use it like this
  Input::RegisterKeyPressCallback(KeyCode::L, [p]() { p->SetActive(false); });
  Input::RegisterKeyPressCallback(KeyCode::O, [p]() { p->SetActive(true); });

  moveCube->AddComponent<KeyTransform>();
  // All the navigation related code is inside of the AITestComponent
  camera->AddComponent<AITestComponent>(
      Math::Rect{0, 0, 10, 10}, Math::Vector2Int{20, 20}, moveCube->transform);
}
