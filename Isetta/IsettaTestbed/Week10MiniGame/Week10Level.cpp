/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/Week10Level.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Scene/Entity.h"
#include "Week10MiniGame/W10NetworkManager.h"
#include "Week10MiniGame/W10UIManager.h"

using namespace Isetta;

void Week10Level::OnLevelLoad() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 0.25f, 3.0f});

  Entity* lightEntity{AddEntity("Light")};
  LightComponent* lightComp = lightEntity->AddComponent<LightComponent, true>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 6, 3.5f}, Math::Vector3{-30, 0, 0},
                            Math::Vector3::one);

  Entity* debug{CREATE_ENTITY("Debug")};
  debug->AddComponent<EditorComponent>();

  Entity* ground{CREATE_ENTITY("Ground")};
  ground->AddComponent<MeshComponent>("blockFencing/Ground.scene.xml");
  ground->SetTransform(Math::Vector3{0, -5.25, 0}, Math::Vector3::zero,
                       Math::Vector3{4.0f, 10.0f, 1});

  Entity* networkManager{CREATE_ENTITY("GameManager")};
  networkManager->AddComponent<W10NetworkManager>();
  networkManager->AddComponent<W10GameManager>();
  networkManager->AddComponent<W10UIManager>();
}
