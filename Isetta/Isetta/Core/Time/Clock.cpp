/*
 * Copyright (c) 2018 Isetta
 */

#include "Core/Time/Clock.h"

namespace Isetta {
Clock::Clock()
    : currentTime{HighResClock::now()},
      deltaMilliseconds{},
      deltaTime(0),
      timeScale{1.f},
      isPause{false} {}

Clock::Clock(const Clock& inClock)
    : currentTime{inClock.currentTime},
      deltaMilliseconds{inClock.deltaMilliseconds},
      deltaTime(0),
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

Clock::Clock(Clock&& inClock) noexcept
    : currentTime{inClock.currentTime},
      deltaMilliseconds(),
      deltaTime(0),
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

void Clock::UpdateTime() {
  deltaTime = 0.f;
  if (!isPause) {
    TimePoint newTime = HighResClock::now();
    deltaMilliseconds =
        std::chrono::duration_cast<Milliseconds>(newTime - currentTime);
    currentTime = newTime;
    deltaTime = deltaMilliseconds.count() / 1000.f * timeScale;
  }
}

float Clock::GetDeltaTime() const { return deltaTime; }
}  // namespace Isetta
