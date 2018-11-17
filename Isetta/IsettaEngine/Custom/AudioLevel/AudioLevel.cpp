/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/AudioLevel/AudioLevel.h"

#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "Graphics/CameraComponent.h"

#include "Audio/AudioListener.h"
#include "Audio/AudioSource.h"
#include "Components/FlyController.h"
#include "Components/GridComponent.h"
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
  camComp->SetProperty<CameraProperty::FOV>(
      CONFIG_VAL(renderConfig.fieldOfView));
  camComp->SetProperty<CameraProperty::NEAR_PLANE>(
      CONFIG_VAL(renderConfig.nearClippingPlane));
  camComp->SetProperty<CameraProperty::FAR_PLANE>(
      CONFIG_VAL(renderConfig.farClippingPlane));

  // Application::Exit();
  cameraEntity->AddComponent<FlyController>();
  cameraEntity->AddComponent<AudioListener>();
  cameraEntity->AddComponent<EscapeExit>();

  Entity* grid{AddEntity("Grid")};
  grid->AddComponent<GridComponent>();

  Entity* audio3D{AddEntity("3DAudio")};
  audio3D->GetTransform()->SetWorldPos(Math::Vector3{0, 0, 0});
  AudioSource* src =
      audio3D->AddComponent<AudioSource>(0b011, 1.f, "Sound/zombie-hit.wav");
  src->Play();

  // Entity* audio2D{AddEntity("2DAudio")};
  // audio2D->AddComponent<AudioSource>();
}
}  // namespace Isetta