/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Time/Clock.h"

#include <ctime>
#include "Core/Debug/Logger.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
Clock::Clock()
    : startTime{HighResClock::now()},
      currentTime{HighResClock::now()},
      deltaTime{},
      elapsedTime(0),
      elapsedUnscaledTime(0),
      timeFrame{0},
      timeScale{1.f},
      isPause{false} {}

Clock::Clock(const Clock& inClock)
    : startTime{inClock.startTime},
      currentTime{inClock.currentTime},
      deltaTime{inClock.deltaTime},
      elapsedTime{inClock.elapsedTime},
      elapsedUnscaledTime{inClock.elapsedUnscaledTime},
      timeFrame(inClock.timeFrame),
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

Clock::Clock(Clock&& inClock) noexcept
    : startTime{inClock.startTime},
      currentTime{inClock.currentTime},
      deltaTime{inClock.deltaTime},
      elapsedTime{inClock.elapsedTime},
      elapsedUnscaledTime{inClock.elapsedUnscaledTime},
      timeFrame{inClock.timeFrame},
      timeScale{inClock.timeScale},
      isPause{inClock.isPause} {}

void Clock::UpdateTime() {
  PROFILE
  ++timeFrame;
  deltaTime = 0.0;
  if (!isPause) {
    TimePoint newTime = HighResClock::now();
    Nanoseconds delta =
        std::chrono::duration_cast<Nanoseconds>(newTime - currentTime);
    Nanoseconds elapse =
        std::chrono::duration_cast<Nanoseconds>(newTime - startTime);

    currentTime = newTime;
    deltaTime = delta.count() * 1e-9 * timeScale;
    elapsedTime += deltaTime;
    elapsedUnscaledTime = elapse.count() * 1e-9;
  }
}

double Clock::GetDeltaTime() const { return deltaTime; }
double Clock::GetElapsedTime() const { return elapsedTime; }
double Clock::GetElapsedUnscaledTime() const { return elapsedUnscaledTime; }
U64 Clock::GetTimeFrame() const { return timeFrame; }

U64 Clock::GetTimestamp() {
  U64 unixTimestamp = std::chrono::seconds(std::time(nullptr)).count();
  return unixTimestamp;
}
}  // namespace Isetta
