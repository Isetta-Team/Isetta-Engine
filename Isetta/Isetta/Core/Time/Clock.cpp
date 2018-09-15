/*
 * Copyright (c) 2018 Isetta
 */

#include "Core/Time/Clock.h"

namespace Isetta {
Clock::Clock()
    : startTime{HighResClock::now()},
      currentTime{HighResClock::now()},
      deltaTime{},
      elapseTime(0),
      elapseUnscaledTime(0),
      timeScale{1.f},
      isPause{false} {}

Clock::Clock(const Clock& inClock)
    : startTime{inClock.startTime},
      currentTime{inClock.currentTime},
      deltaTime(inClock.deltaTime),
      elapseTime{inClock.elapseTime},
      elapseUnscaledTime{inClock.elapseUnscaledTime},
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

Clock::Clock(Clock&& inClock) noexcept
    : startTime{inClock.startTime},
      currentTime{inClock.currentTime},
      deltaTime{inClock.deltaTime},
      elapseTime{inClock.elapseTime},
      elapseUnscaledTime{inClock.elapseUnscaledTime},
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
    elapseTime += deltaTime;
    elapseTime = elapse.count() / 1000.f;
  }
}

double Clock::GetDeltaTime() const { return deltaTime; }
double Clock::GetElapseTime() const { return elapseTime; }
double Clock::GetElapseUnscaledTime() const { return elapseUnscaledTime; }
}  // namespace Isetta
