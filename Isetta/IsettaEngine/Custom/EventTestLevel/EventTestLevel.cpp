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

void EventTestLevel::OnLevelLoad() {
  Entity* cameraEntity = Entity::CreateEntity("Camera");
  cameraEntity->AddComponent<CameraComponent>();
  cameraEntity->SetTransform(Math::Vector3{0, 5, 10}, Math::Vector3{-15, 0, 0},
                             Math::Vector3::one);

  Entity* senderEntity = Entity::CreateEntity("SenderEntity");
  senderEntity->AddComponent<EventSenderComponent>();

  Entity* listenerEntity = Entity::CreateEntity("ListenerEntity");
  listenerEntity->AddComponent<EventListenerComponent>();
}
