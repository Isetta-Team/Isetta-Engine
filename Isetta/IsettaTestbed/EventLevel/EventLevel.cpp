/*
 * Copyright (c) 2018 Isetta
 */
#include "EventLevel.h"
#include "EventListenerComponent.h"
#include "EventSenderComponent.h"

using namespace Isetta;

void EventLevel::Load() {
  Entity* cameraEntity = Entity::Instantiate("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  // Send/create event
  Entity* senderEntity = Entity::Instantiate("SenderEntity");
  senderEntity->AddComponent<EventSenderComponent>();

  // Listen for event
  Entity* listenerEntity = Entity::Instantiate("ListenerEntity");
  listenerEntity->AddComponent<EventListenerComponent>();
}
