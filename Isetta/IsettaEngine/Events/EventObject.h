/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <functional>
#include <variant>
#include "Core/DataStructures/Array.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
using EventParam =
    std::variant<bool, int, float, std::string, Math::Vector3, void*>;

enum class ISETTA_API EventPriority {
  LOW = 3000,
  MEDIUM = 2000,
  HIGH = 1000,
  EMERGENT = 0
};

struct ISETTA_API EventObject {
  std::string eventName;
  U64 timeFrame;
  EventPriority eventPriority;
  Array<EventParam> eventParams;

  EventObject() = default;
  EventObject(std::string name, U64 frame, EventPriority priority,
              Array<EventParam> params);
  EventObject(std::string name, Array<EventParam> params);
  EventObject(const EventObject& other);
  EventObject(EventObject&& other) noexcept;
  EventObject& operator=(const EventObject& other);
  EventObject& operator=(EventObject&& other) noexcept;

  bool operator==(const EventObject& other) const;
  bool operator!=(const EventObject& other) const;
  bool operator>(const EventObject& rhs) const;

  static std::function<bool(const EventObject&, const EventObject&)>
      queueComparer;
};
}  // namespace Isetta
