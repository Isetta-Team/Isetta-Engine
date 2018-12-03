/*
 * Copyright (c) 2018 Isetta
 */
#include "ExampleLevel.h"

#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "ExampleComponent.h"

namespace Isetta {

void ExampleLevel::Load() {
  // Instantiate entity
  Entity* cameraEntity = Entity::Instantiate("Camera");
  // Add CameraComponent: what renders anything on screen
  //  NEED Camera each level
  cameraEntity->AddComponent<CameraComponent>();
  // Sets the transform of the entity: WorldPosition, WorldRotation, LocalScale
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  // Add FlyController: see FlyController for more details
  cameraEntity->AddComponent<FlyController>();

  Entity* lightEntity = Entity::Instantiate("Light");
  // Add LightComponent: settings for light component
  // can be found in LightComponent.h
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity* man = Entity::Instantiate("PushAnim");
  man->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 90, 0});
  // Add and cache MeshComponent: argument is filepath of scene file of mesh
  MeshComponent* pushMesh =
      man->AddComponent<MeshComponent>("Example/Pushing.scene.xml");
  // Add and cache AnimationComponent: arguments are:
  //  - filepath of animation file (.anim)
  //  - layer of animation
  //  - startingNode of animation system
  //  - whether the animation is additive
  AnimationComponent* ani = man->AddComponent<AnimationComponent>(pushMesh);
  ani->AddAnimation("Example/Pushing.anim", 0, "", false);

  Entity* customEntity = Entity::Instantiate("custom");
  // Add ExampleComponent: empty component just demoing components
  customEntity->AddComponent<ExampleComponent>();
  // Add GridComponent: displays debug drawing grid
  customEntity->AddComponent<GridComponent>();
}
}  // namespace Isetta