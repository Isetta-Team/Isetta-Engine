/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/Week10Level.h"
#include "Components/Editor/Editor.h"
#include "Components/FlyController.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"
#include "Week10MiniGame/W10NetworkManager.h"

using namespace Isetta;

void Week10Level::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 0.25f, 3.0f});
  camComp->SetProperty<CameraComponent::Property::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraComponent::Property::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraComponent::Property::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent, true>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 6, 3.5f}, Math::Vector3{-30, 0, 0},
                            Math::Vector3::one);
  lightComp->SetProperty<LightComponent::Property::RADIUS>(2500);
  lightComp->SetProperty<LightComponent::Property::FOV>(180);
  lightComp->SetProperty<LightComponent::Property::COLOR>(Color::white);
  lightComp->SetProperty<LightComponent::Property::COLOR_MULTIPLIER>(1.0f);
  lightComp->SetProperty<LightComponent::Property::SHADOW_MAP_COUNT>(1);
  lightComp->SetProperty<LightComponent::Property::SHADOW_MAP_BIAS>(0.01f);

  Entity* debug{ADD_ENTITY("Debug")};
  debug->AddComponent<Editor>(true);

  Entity* ground{ADD_ENTITY("Ground")};
  ground->AddComponent<MeshComponent>("blockFencing/Ground.scene.xml");
  ground->SetTransform(Math::Vector3{0, -5.25, 0}, Math::Vector3::zero,
                       Math::Vector3{4.0f, 10.0f, 1});

  Entity* networkManager{ADD_ENTITY("GameManager")};
  networkManager->AddComponent<W10NetworkManager>();
  networkManager->AddComponent<W10GameManager>();
}
