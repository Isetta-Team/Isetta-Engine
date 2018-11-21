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
  Entity* cameraEntity{Entity::CreateEntity("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 0.25f, 3.0f});

  Entity* lightEntity{Entity::CreateEntity("Light")};
  lightEntity->AddComponent<LightComponent>(
      "materials/light.material.xml", "LIGHT_1");
  lightEntity->SetTransform(Math::Vector3{0, 6, 3.5f}, Math::Vector3{-30, 0, 0},
                            Math::Vector3::one);

  Entity* debug{Entity::CreateEntity("Debug")};
  debug->AddComponent<EditorComponent>();

  Entity* ground{Entity::CreateEntity("Ground")};
  ground->AddComponent<MeshComponent>("blockFencing/Ground.scene.xml");
  ground->SetTransform(Math::Vector3{0, -5.25, 0}, Math::Vector3::zero,
                       Math::Vector3{4.0f, 10.0f, 1});

  Entity* networkManager{Entity::CreateEntity("GameManager")};
  networkManager->AddComponent<W10NetworkManager>();
  networkManager->AddComponent<W10GameManager>();
  networkManager->AddComponent<W10UIManager>();
}
