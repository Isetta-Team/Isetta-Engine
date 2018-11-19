/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AudioLevel/AudioLevel.h"

#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"
#include "Input/Input.h"

#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/AudioPlay.h"
#include "Custom/EscapeExit.h"

namespace Isetta {
using CameraProperty = CameraComponent::Property;
using AudioProperty = AudioSource::Property;

void AudioLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  // cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0,
  // 0},
  //                           Math::Vector3::one);

  // Application::Exit();
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<AudioListener>();
  cameraEntity->AddComponent<EscapeExit>();

  Entity* grid{AddEntity("Grid")};
  grid->AddComponent<GridComponent>();

  Entity* audio3D{AddEntity("3DAudio")};
  audio3D->transform->SetWorldPos(Math::Vector3{0, 0, 0});
  AudioSource* src3D =
      audio3D->AddComponent<AudioSource>(0b011, "Sound/zombie-hit.wav");
  audio3D->AddComponent<AudioPlay>(KeyCode::NUM3, src3D);

  Entity* audio2D{AddEntity("2DAudio")};
  AudioSource* src2D =
      audio2D->AddComponent<AudioSource>(0b010, "Sound/zombie-death.mp3");
  audio2D->AddComponent<AudioPlay>(KeyCode::NUM2, src2D);
  src2D->SetVolume(0.5f);
}
}  // namespace Isetta