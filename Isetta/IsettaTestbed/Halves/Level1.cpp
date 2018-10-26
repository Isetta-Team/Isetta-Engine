/*
 * Copyright (c) 2018 Isetta
 */
#include "Level1.h"
#include "CameraController.h"
#include "Components/FlyController.h"
#include "GameManager.h"
#include "PlayerController.h"
#include "Core/Color.h"
#include "Core/Config/Config.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Scene/Entity.h"

namespace Isetta {

using LightProperty = LightComponent::Property;
using CameraProperty = CameraComponent::Property;

void Level1::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  cameraEntity->AddComponent<CameraController, true>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent, true>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 200, 600},
                            Math::Vector3{-30, 0, 0});
  lightComp->SetProperty<LightProperty::RADIUS>(2500);
  lightComp->SetProperty<LightProperty::FOV>(180);
  lightComp->SetProperty<LightProperty::COLOR>(Color::white);
  lightComp->SetProperty<LightProperty::COLOR_MULTIPLIER>(1.5f);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightProperty::SHADOW_MAP_BIAS>(0.01f);

  Entity* player{AddEntity("Player")};
  player->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 90, 0},
                       0.03f * Math::Vector3::one);
  MeshComponent* playerMesh =
      player->AddComponent<MeshComponent, true>("Soldier/Soldier.scene.xml");
  player->AddComponent<PlayerController, true>();

  AnimationComponent* ani =
      player->AddComponent<AnimationComponent, true>(playerMesh);
  ani->AddAnimation("Soldier/Soldier_Idle.anim", 0, "", false);
  ani->AddAnimation("Soldier/Soldier.anim", 0, "", false);

  Entity* ground{AddEntity("Ground")};
  ground->AddComponent<MeshComponent, true>("Ground/Level.scene.xml");

  Entity* gameManager{AddEntity("Game Manager")};
  gameManager->AddComponent<GameManager, true>();

  // for (int i = 0; i < 10; i++) {
  // Entity* zombie {AddEntity("Zombie")};
  // zombie->SetTransform(Math::Vector3::forward, Math::Vector3::zero,
  // Math::Vector3::one * 0.01f); MeshComponent* mesh =
  // zombie->AddComponent<MeshComponent>(true, "Zombie/Zombie.scene.xml");
  // AnimationComponent* animation =
  // zombie->AddComponent<AnimationComponent>(true, mesh);
  // animation->AddAnimation("Zombie/Zombie.anim", 0, "", false);
  // zombie->AddComponent<Zombie>();
  // }
}
}  // namespace Isetta