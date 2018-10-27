/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include <variant>
#include <vector>
#include "Core/Math/Vector3.h"
#include <functional>

namespace Isetta {
using EventParam = std::variant<bool, int, float, Math::Vector3, void*>;

enum class EventPriority {
  LOW = 3000,
  MEDIUM = 2000,
  HIGH = 1000,
  EMERGENT = 0
};

struct EventObject {
  std::string eventName;
  int timeFrame;
  EventPriority eventPriority;
  std::vector<EventParam> eventParams;

  EventObject(std::string name, int frame, EventPriority priority,
              std::vector<EventParam> params);
  EventObject(const EventObject& other);
  EventObject(EventObject&& other);
  EventObject& operator=(const EventObject& other);
  EventObject& operator=(EventObject&& other);

  bool operator==(const EventObject& other) const;
  bool operator!=(const EventObject& other) const;

  static std::function<bool(const EventObject&, const EventObject&)> queueComparer;
};
}  // namespace Isetta
