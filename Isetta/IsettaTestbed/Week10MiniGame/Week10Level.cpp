/*
 * Copyright (c) 2018 Isetta
 */
#include "Week10MiniGame/Week10Level.h"
#include "Components/Editor/EditorComponent.h"
#include "Week10MiniGame/W10NetworkManager.h"
#include "Week10MiniGame/W10UIManager.h"

using namespace Isetta;

void Week10Level::Load() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 0.25f, 3.0f});

  Entity* lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 6, 3.5f}, Math::Vector3{-30, 0, 0},
                            Math::Vector3::one);

  Entity* debug{Entity::Instantiate("Debug")};
  debug->AddComponent<EditorComponent>();

  Entity* ground{Entity::Instantiate("Ground")};
  ground->AddComponent<MeshComponent>("Week10/Ground.scene.xml");
  ground->SetTransform(Math::Vector3{0, -5.25, 0}, Math::Vector3::zero,
                       Math::Vector3{4.0f, 10.0f, 1});

  Entity* networkManager{Entity::Instantiate("GameManager")};
  networkManager->AddComponent<W10NetworkManager>();
  networkManager->AddComponent<W10GameManager>();
  networkManager->AddComponent<W10UIManager>();
}
