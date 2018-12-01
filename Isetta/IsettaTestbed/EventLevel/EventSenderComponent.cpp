/*
 * Copyright (c) 2018 Isetta
 */
#include "EventSenderComponent.h"

void EventSenderComponent::Start() {
  handleA = Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::A, [&]() {
    // Define an event object with name, parameters, time frame and priority
    Isetta::EventParam param{std::string{"RaiseEvent In Queue"}};
    Isetta::EventObject eventObject{"RaiseEvent",
                                    Isetta::Time::GetFrameCount(),
                                    Isetta::EventPriority::MEDIUM,
                                    {param}};
    // Raise a queued event
    Isetta::Events::Instance().RaiseQueuedEvent(eventObject);
    Isetta::Input::UnregisterKeyPressCallback(Isetta::KeyCode::A, handleA);
  });
  handleB = Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::S, [&]() {
    Isetta::EventParam param{std::string{"RaiseEvent Immediately"}};
    Isetta::EventObject eventObject{"RaiseEvent",
                                    Isetta::Time::GetFrameCount(),
                                    Isetta::EventPriority::MEDIUM,
                                    {param}};
    // Raise an immediate event
    Isetta::Events::Instance().RaiseImmediateEvent(eventObject);
    Isetta::Input::UnregisterKeyPressCallback(Isetta::KeyCode::S, handleB);
  });
  handleC = Isetta::Input::RegisterKeyPressCallback(Isetta::KeyCode::D, [&]() {
    Isetta::EventParam param1{std::string{"RaiseEvent High"}};
    Isetta::EventParam param2{std::string{"RaiseEvent Low"}};
    Isetta::EventParam param3{std::string{"RaiseEvent Late"}};

    Isetta::EventObject eventObject1{"RaiseEvent",
                                     Isetta::Time::GetFrameCount(),
                                     Isetta::EventPriority::HIGH,
                                     {param1}};
    Isetta::EventObject eventObject2{"RaiseEvent",
                                     Isetta::Time::GetFrameCount(),
                                     Isetta::EventPriority::LOW,
                                     {param2}};
    Isetta::EventObject eventObject3{"RaiseEvent",
                                     Isetta::Time::GetFrameCount() + 5,
                                     Isetta::EventPriority::HIGH,
                                     {param3}};

    Isetta::Events::Instance().RaiseQueuedEvent(eventObject1);
    Isetta::Events::Instance().RaiseQueuedEvent(eventObject2);
    Isetta::Events::Instance().RaiseQueuedEvent(eventObject3);
    // The first event should go first, then the second and the third
    // It's sorted based on it's time frame first and the priority second
    Isetta::Input::UnregisterKeyPressCallback(Isetta::KeyCode::D, handleC);
  });
}
