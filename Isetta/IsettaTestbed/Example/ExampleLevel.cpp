/*
 * Copyright (c) 2018 Isetta
 */
#include "ExampleLevel.h"

#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "ExampleComponent.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Scene/Entity.h"

namespace Isetta {

void ExampleLevel::OnLevelLoad() {
  Entity* cameraEntity = Entity::CreateEntity("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();

  Entity* lightEntity = Entity::CreateEntity("Light");
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* man = Entity::CreateEntity("PushAnim");
  man->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 90, 0});
  // man->AddComponent<PlayerController, true>();
  MeshComponent* pushMesh =
      man->AddComponent<MeshComponent>("push/Pushing.scene.xml");

  AnimationComponent* ani = man->AddComponent<AnimationComponent>(pushMesh);
  ani->AddAnimation("push/Pushing.anim", 0, "", false);

  Entity* customEntity = Entity::CreateEntity("custom");
  customEntity->AddComponent<ExampleComponent>();
  customEntity->AddComponent<GridComponent>();
}
}  // namespace Isetta