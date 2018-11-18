/*
 * Copyright (c) 2018 Isetta
 */
#include "ExampleLevel.h"

#include "ExampleComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Scene/Entity.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void ExampleLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController, true>();

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent, true>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* man{AddEntity("PushAnim")};
  man->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 90, 0});
  // man->AddComponent<PlayerController, true>();
  MeshComponent* pushMesh =
      man->AddComponent<MeshComponent, true>("push/Pushing.scene.xml");

  AnimationComponent* ani =
      man->AddComponent<AnimationComponent, true>(pushMesh);
  ani->AddAnimation("push/Pushing.anim", 0, "", false);

  Entity* customEntity{AddEntity("custom")};
  customEntity->AddComponent<ExampleComponent, true>();
  customEntity->AddComponent<GridComponent>();
}
}  // namespace Isetta