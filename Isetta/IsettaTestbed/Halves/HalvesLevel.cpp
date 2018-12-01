/*
 * Copyright (c) 2018 Isetta
 */
#include "HalvesLevel.h"

#include "CameraController.h"
#include "Components/Editor/EditorComponent.h"
#include "Components/FlyController.h"
#include "Custom/EscapeExit.h"
#include "GameManager.h"
#include "PlayerController.h"

namespace Isetta {

void HalvesLevel::Load() {
  Font::AddFontFromFile("Fonts\\CONSOLA.TTF", 13.0f, "Consola");

  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraController>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  cameraEntity->AddComponent<CameraComponent>();

  // We need an audio listener for 3d audio
  cameraEntity->AddComponent<AudioListener>();

  Entity* lightEntity{Entity::Instantiate("Light")};
  lightEntity->AddComponent<LightComponent>();
  lightEntity->SetTransform(Math::Vector3{0, 200, 600},
                            Math::Vector3{-30, 0, 0});

  // instantiate player and add mesh and animations
  Entity* player{Entity::Instantiate("Player")};
  player->SetTransform(Math::Vector3{0, 0, 0}, Math::Vector3{0, 90, 0},
                       0.01f * Math::Vector3::one);
  MeshComponent* playerMesh =
      player->AddComponent<MeshComponent>("Halves/Soldier/Soldier.scene.xml");
  AnimationComponent* playerAnimationComp =
      player->AddComponent<AnimationComponent>(playerMesh);
  playerAnimationComp->AddAnimation("Halves/Soldier/Soldier_Idle.anim", 0, "", false);
  playerAnimationComp->AddAnimation("Halves/Soldier/Soldier.anim", 0, "", false);
  player->AddComponent<PlayerController>();

  // instantiate the ground
  Entity* ground{Entity::Instantiate("Ground")};
  ground->AddComponent<MeshComponent>("Halves/Ground/Level.scene.xml");

  // instantiate the game manager, also add editor component to it
  Entity* gameManager{Entity::Instantiate("Game Manager")};
  gameManager->AddComponent<GameManager>();
  gameManager->AddComponent<EscapeExit>();
  gameManager->AddComponent<EditorComponent>();
}
}  // namespace Isetta