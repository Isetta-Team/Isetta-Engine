/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AudioLevel/AudioLevel.h"

#include "Core/Config/Config.h"
#include "Core/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/AudioPlay.h"
#include "Custom/EscapeExit.h"

namespace Isetta {
using AudioProperty = AudioSource::Property;

void AudioLevel::OnLevelLoad() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();

  // Application::Exit();
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<AudioListener>();
  cameraEntity->AddComponent<EscapeExit>();

  Entity* grid{Entity::Instantiate("Grid")};
  grid->AddComponent<GridComponent>();

  Entity* audio3D{Entity::Instantiate("3DAudio")};
  audio3D->transform->SetWorldPos(Math::Vector3{0, 0, 0});
  AudioSource* src3D = audio3D->AddComponent<AudioSource>(
      0b001, AudioClip::Load("Sound/zombie-hit.wav"));
  audio3D->AddComponent<AudioPlay>(KeyCode::NUM3, src3D);

  Entity* audio2D{Entity::Instantiate("2DAudio")};
  AudioSource* src2D = audio2D->AddComponent<AudioSource>(
      0b010, AudioClip::Load("Sound/zombie-death.mp3"));
  audio2D->AddComponent<AudioPlay>(KeyCode::NUM2, src2D);
  src2D->SetVolume(0.5f);
}
}  // namespace Isetta