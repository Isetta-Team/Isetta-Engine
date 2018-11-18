/*
 * Copyright (c) 2018 Isetta
 */
#include "EventTestLevel.h"
#include "Application.h"
#include "Core/Config/Config.h"
#include "Custom/IsettaCore.h"
#include "EventListenerComponent.h"
#include "EventSenderComponent.h"
#include "Graphics/CameraComponent.h"

using namespace Isetta;
using CameraProperty = CameraComponent::Property;

void EventTestLevel::LoadLevel() {
  Entity* cameraEntity{AddEntity("Camera")};
  CameraComponent* camComp =
      cameraEntity->AddComponent<CameraComponent, true>("Camera");
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  Entity* senderEntity{ADD_ENTITY("SenderEntity")};
  senderEntity->AddComponent<EventSenderComponent>();

  Entity* listenerEntity{ADD_ENTITY("ListenerEntity")};
  listenerEntity->AddComponent<EventListenerComponent>();
}
