/*
 * Copyright (c) 2018 Isetta
 */

#include "Core/Time/Clock.h"

namespace Isetta {
Clock::Clock()
    : startTime{HighResClock::now()},
      currentTime{HighResClock::now()},
      deltaTime{},
      elapsedTime(0),
      elapsedUnscaledTime(0),
      timeScale{1.f},
      isPause{false} {}

Clock::Clock(const Clock& inClock)
    : startTime{inClock.startTime},
      currentTime{inClock.currentTime},
      deltaTime(inClock.deltaTime),
      elapsedTime{inClock.elapsedTime},
      elapsedUnscaledTime{inClock.elapsedUnscaledTime},
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

Clock::Clock(Clock&& inClock) noexcept
    : startTime{inClock.startTime},
      currentTime{inClock.currentTime},
      deltaTime{inClock.deltaTime},
      elapsedTime{inClock.elapsedTime},
      elapsedUnscaledTime{inClock.elapsedUnscaledTime},
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

void Clock::UpdateTime() {
  deltaTime = 0.0;
  if (!isPause) {
    TimePoint newTime = HighResClock::now();
    Milliseconds delta =
        std::chrono::duration_cast<Milliseconds>(newTime - currentTime);
    Milliseconds elapse =
        std::chrono::duration_cast<Milliseconds>(newTime - startTime);
    currentTime = newTime;
    deltaTime = delta.count() / 1000.f * timeScale;
    elapsedTime += deltaTime;
    elapsedTime = elapse.count() / 1000.f;
  }
}

double Clock::GetDeltaTime() const { return deltaTime; }
double Clock::GetElapsedTime() const { return elapsedTime; }
double Clock::GetElapsedUnscaledTime() const { return elapsedUnscaledTime; }
}  // namespace Isetta
