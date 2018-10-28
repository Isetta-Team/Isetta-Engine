/*
 * Copyright (c) 2018 Isetta
 */
#include "Custom/EventTestLevel/EventSenderComponent.h"
#include "Core/Time/Time.h"
#include "Events/EventObject.h"
#include "Events/Events.h"
#include "Input/Input.h"

void EventSenderComponent::Start() {
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::A, []() {
    Isetta::EventParam param{"RaiseEvent In Queue"};
    Isetta::EventObject eventObject{"RaiseEvent",
                                    Isetta::Time::GetTimeFrame(),
                                    Isetta::EventPriority::MEDIUM,
                                    {param}};
    Isetta::Events::Instance().RaiseEvent(eventObject);
  });
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::S, []() {
    Isetta::EventParam param{"RaiseEvent Immediately"};
    Isetta::EventObject eventObject{"RaiseEvent",
                                    Isetta::Time::GetTimeFrame(),
                                    Isetta::EventPriority::MEDIUM,
                                    {param}};
    Isetta::Events::Instance().RaiseEventImmediate(eventObject);
  });
  Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::D, []() {
    Isetta::EventParam param1{"RaiseEvent High"};
    Isetta::EventParam param2{"RaiseEvent Low"};
    Isetta::EventParam param3{"RaiseEvent Late"};

    Isetta::EventObject eventObject1{"RaiseEvent",
                                     Isetta::Time::GetTimeFrame(),
                                     Isetta::EventPriority::HIGH,
                                     {param1}};
    Isetta::EventObject eventObject2{"RaiseEvent",
                                     Isetta::Time::GetTimeFrame(),
                                     Isetta::EventPriority::LOW,
                                     {param2}};
    Isetta::EventObject eventObject3{"RaiseEvent",
                                     Isetta::Time::GetTimeFrame() + 5,
                                     Isetta::EventPriority::HIGH,
                                     {param3}};

    Isetta::Events::Instance().RaiseEvent(eventObject1);
    Isetta::Events::Instance().RaiseEvent(eventObject2);
    Isetta::Events::Instance().RaiseEvent(eventObject3);
  });
}
