/*
 * Copyright (c) 2018 Isetta
 */
#include "AudioLevel.h"

#include "AudioLevel/AudioPlay.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
#include "Custom/EscapeExit.h"

namespace Isetta {
using AudioProperty = AudioSource::Property;

void AudioLevel::Load() {
  Entity* cameraEntity{Entity::Instantiate("Camera")};
  cameraEntity->AddComponent<CameraComponent>();

  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<EscapeExit>();
  // AudioListener needed for audio to be spatial (3D)
  // [Warning] Only have 1 per level
  cameraEntity->AddComponent<AudioListener>();

  Entity* grid{Entity::Instantiate("Grid")};
  grid->AddComponent<GridComponent>();

  // 3D AudioSource component
  Entity* audio3D{Entity::Instantiate("3DAudio")};
  audio3D->transform->SetWorldPos(Math::Vector3{0, 0, 0});
  // AudioClip::Load loads the audio in the filepath under resource_path
  // AudioSource constructor parameters:
  // 0b001 - properties of AudioSource 0b=binary then from left to right:
  // IS_MUTE, IS_LOOP, IS_3D
  // This AudioSource is NOT muted, NOT looping, but IS 3D
  AudioSource* src3D = audio3D->AddComponent<AudioSource>(
      0b001, AudioClip::Load("Sound/zombie-hit.wav"));
  // AudioPlay component which plays AudioSource on KeyCode press
  audio3D->AddComponent<AudioPlay>(KeyCode::NUM3, src3D);

  // 2D AudioSource component
  Entity* audio2D{Entity::Instantiate("2DAudio")};
  // AudioSource properties are NOT muted, IS looping, NOT 3D
  AudioSource* src2D = audio2D->AddComponent<AudioSource>(
      0b010, AudioClip::Load("Sound/zombie-death.mp3"));
  audio2D->AddComponent<AudioPlay>(KeyCode::NUM2, src2D);
  src2D->SetVolume(0.5f);
}
}  // namespace Isetta