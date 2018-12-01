/*
 * Copyright (c) 2018 Isetta
 */
#include <Events/Events.h>
#include <execution>
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "Core/Time/Time.h"
#include "brofiler/ProfilerCore/Brofiler.h"

using namespace Isetta;
U16 Events::totalListeners = 0;
void Isetta::Events::ShutDown() {
  callbackMap.clear();
  eventQueue.~PriorityQueue();
  instance = nullptr;
}
void Events::RaiseQueuedEvent(const EventObject& eventObject) {
  PROFILE
  eventQueue.Push(eventObject);
}

void Events::RaiseImmediateEvent(const EventObject& eventObject) {
  PROFILE
  StringId eventNameId{SID(eventObject.eventName.c_str())};
  if (callbackMap.count(eventNameId) > 0) {
    // prevent unregistering the callback from screwing up the range-based for
    // loop
    Array<std::pair<U16, Action<EventObject>>> callbacks{
        callbackMap.at(eventNameId)};
    for (const auto& callbackPair : callbacks) {
      callbackPair.second(eventObject);
    }
  } else {
    LOG_WARNING(Debug::Channel::Gameplay, Debug::Verbosity::Warning,
                "Event %s has no listener.", eventObject.eventName.c_str());
  }
}

U16 Events::RegisterEventListener(std::string_view eventName,
                                  const Action<EventObject>& callback) {
  StringId eventNameId{SID(eventName.data())};
  U16 handle = totalListeners++;
  if (callbackMap.count(eventNameId) > 0) {
    callbackMap.at(eventNameId).EmplaceBack(std::make_pair(handle, callback));
  } else {
    callbackMap.insert(std::make_pair(
        eventNameId,
        std::vector<CallbackPair>{std::make_pair(handle, callback)}));
  }
  return handle;
}

void Events::UnregisterEventListener(std::string_view eventName,
                                     U16 eventListenerHandle) {
  StringId eventNameId{SID(eventName.data())};
  if (callbackMap.count(eventNameId) > 0) {
    Array<CallbackPair>& callbacks = callbackMap.at(eventNameId);
    callbacks.Erase(
        std::remove_if(std::begin(callbacks), std::end(callbacks),
                       [eventListenerHandle](const CallbackPair& pair) {
                         return pair.first == eventListenerHandle;
                       }),
        std::end(callbacks));
  } else {
    LOG_WARNING(Debug::Channel::Gameplay, Debug::Verbosity::Warning,
                "Event %s has no listener.", eventName);
  }
}

void Events::Clear() {
  eventQueue.Clear();
  callbackMap.clear();
}

void Events::Update() {
  BROFILER_CATEGORY("Event Update", Profiler::Color::Lavender);

  while (!eventQueue.IsEmpty()) {
    EventObject currEvent = eventQueue.Top();
    if (currEvent.timeFrame > Time::GetFrameCount()) {
      break;
    }
    eventQueue.Pop();
    RaiseImmediateEvent(currEvent);
  }
}
