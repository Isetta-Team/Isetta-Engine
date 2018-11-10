/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <chrono>
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"

namespace Isetta {
class ISETTA_API Clock {
  using Nanoseconds = std::chrono::nanoseconds;
  using HighResClock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<HighResClock>;

  TimePoint startTime;
  TimePoint currentTime;
  double deltaTime;
  double elapsedTime;
  double elapsedUnscaledTime;
  U64 timeFrame;

 public:
  Clock();

  Clock(const Clock& inClock);
  Clock(Clock&& inClock) noexcept;

  Clock& operator=(const Clock& inClock) = delete;
  Clock& operator=(Clock&& inClock) = delete;

  /**
   * \brief The time scale of this clock
   */
  float timeScale;
  /**
   * \brief Check if the clock is paused
   */
  bool isPause;

  /**
   * \brief Update the clock time by realtime
   */
  void UpdateTime();
  /**
   * \brief Get the delta time from last update
   */
  double GetDeltaTime() const;
  /**
   * \brief Get elapsed time since the clock started
   */
  double GetElapsedTime() const;
  /**
   * \brief Get unscaled elapsed time since clock started
   */
  double GetElapsedUnscaledTime() const;

  U64 GetTimeFrame() const;

  /**
   * \brief Get the real timestamp
   */
  static U64 GetTimestamp();
};

}  // namespace Isetta
