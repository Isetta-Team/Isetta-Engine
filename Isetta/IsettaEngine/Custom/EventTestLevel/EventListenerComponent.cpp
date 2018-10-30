/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EventTestLevel/EventListenerComponent.h"
#include "Core/Debug/Logger.h"
#include "Events/Events.h"

void EventListenerComponent::Start() {
  Isetta::Events::Instance().RegisterEventListener(
      "RaiseEvent", [](const Isetta::EventObject& eventObject) {
        const char* message{std::get<const char*>(eventObject.eventParams[0])};
        LOG_INFO(Isetta::Debug::Channel::Gameplay,
                 Isetta::Debug::Verbosity::Info, "Event: %s", message);
      });
}
