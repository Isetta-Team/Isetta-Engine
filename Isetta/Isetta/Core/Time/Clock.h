/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <chrono>
#include "Core/IsettaAlias.h"

namespace Isetta {
class Clock {
  using Milliseconds = std::chrono::milliseconds;
  using HighResClock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<HighResClock>;

  TimePoint startTime;
  TimePoint currentTime;
  double deltaTime;
  double elapsedTime;
  double elapsedUnscaledTime;

 public:
  Clock();

  Clock(const Clock& inClock);
  Clock(Clock&& inClock) noexcept;

  Clock& operator=(const Clock& inClock) = delete;
  Clock& operator=(Clock&& inClock) = delete;

  float timeScale;
  bool isPause;

  void UpdateTime();
  double GetDeltaTime() const;
  double GetElapsedTime() const;
  double GetElapsedUnscaledTime() const;
};

}  // namespace Isetta
