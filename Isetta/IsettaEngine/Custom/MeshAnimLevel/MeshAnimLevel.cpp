/*
 * Copyright (c) 2018 Isetta
 */
#include "MeshAnimLevel.h"

#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"

namespace Isetta {

void MeshAnimLevel::OnLevelLoad() {
  Entity *cameraEntity{CREATE_ENTITY("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);
  cameraEntity->AddComponent<FlyController>();

  Entity *lightEntity{CREATE_ENTITY("Light")};
  lightEntity->AddComponent<LightComponent>("materials/light.material.xml",
                                            "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600}, Math::Vector3::zero,
                            Math::Vector3::one);

  Entity *grid{CREATE_ENTITY("Grid")};
  grid->AddComponent<GridComponent>();
  grid->AddComponent<EditorComponent>();

  Entity *zombie{CREATE_ENTITY("Zombie")};
  MeshComponent *mesh =
      zombie->AddComponent<MeshComponent>("Zombie/Zombie.scene.xml");
  AnimationComponent *animation =
      zombie->AddComponent<AnimationComponent>(mesh);
  animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
  zombie->SetTransform(Math::Vector3::zero, Math::Vector3::zero,
                       Math::Vector3::one * 0.01f);

  Entity *cube{CREATE_ENTITY("Cube")};
  cube->AddComponent<MeshComponent>("primitives/Cube.scene.xml");
  cube->transform->SetParent(zombie->transform);
  cube->transform->SetLocalPos(2.f * Math::Vector3::up);
}
}  // namespace Isetta