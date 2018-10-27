/*
 * Copyright (c) 2018 Isetta
 */
#include <Events/EventObject.h>

namespace Isetta {

EventObject::EventObject(std::string name, int frame, EventPriority priority,
                         std::vector<EventParam> params)
    : eventName{name},
      timeFrame{frame},
      eventPriority{priority},
      eventParams{params} {}

EventObject::EventObject(const EventObject& other)
    : eventName{other.eventName},
      timeFrame{other.timeFrame},
      eventPriority{other.eventPriority},
      eventParams{other.eventParams} {}

EventObject::EventObject(EventObject&& other)
    : eventName{std::move(other.eventName)},
      timeFrame{std::move(other.timeFrame)},
      eventPriority{std::move(other.eventPriority)},
      eventParams{std::move(other.eventParams)} {}

EventObject& EventObject::operator=(const EventObject& other) {
  if (other == *this) return *this;
  eventName = other.eventName;
  timeFrame = other.timeFrame;
  eventPriority = other.eventPriority;
  eventParams = other.eventParams;
  return *this;
}

EventObject& EventObject::operator=(EventObject&& other) {
  if (other == *this) return *this;
  eventName = std::move(other.eventName);
  timeFrame = std::move(other.timeFrame);
  eventPriority = std::move(other.eventPriority);
  eventParams = std::move(other.eventParams);
  return *this;
}

bool EventObject::operator==(const EventObject& other) const {
  return eventName == other.eventName && timeFrame == other.timeFrame &&
         eventPriority == other.eventPriority &&
         eventParams == other.eventParams;
}

bool EventObject::operator!=(const EventObject& other) const {
  return !(*this == other);
}

std::function<bool(EventObject, EventObject)> queueComparer =
    [](const EventObject& lhs, const EventObject& rhs) -> bool {
  return (lhs.timeFrame < rhs.timeFrame) ||
         (lhs.timeFrame == rhs.timeFrame &&
          lhs.eventPriority < rhs.eventPriority);
};
}  // namespace Isetta
