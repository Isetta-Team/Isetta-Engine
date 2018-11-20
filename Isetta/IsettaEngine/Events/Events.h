/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataStructures/PriorityQueue.h"
#include "Core/IsettaAlias.h"
#include "Events/EventObject.h"
#include "SID/sid.h"

namespace Isetta {
using CallbackPair = std::pair<U16, Action<EventObject>>;
class ISETTA_API Events {
 public:
  static Events& Instance() { return *instance; }

  void StartUp() { instance = this; }
  void ShutDown();

  void RaiseQueuedEvent(const EventObject& eventObject);
  void RaiseImmediateEvent(const EventObject& eventObject);

  U16 RegisterEventListener(std::string_view eventName,
                            const Action<EventObject>& callback);
  void UnregisterEventListener(std::string_view eventName,
                               U16 eventListenerHandle);

  void Clear();

 private:
  inline static Events* instance;

  Events() = default;
  PriorityQueue<EventObject, Greater> eventQueue;
  std::unordered_map<StringId, Array<CallbackPair>> callbackMap;

  void Update();

  static U16 totalListeners;

  friend class EngineLoop;
  friend class StackAllocator;
};
}  // namespace Isetta