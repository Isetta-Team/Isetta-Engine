/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <chrono>
#include "core/IsettaAlias.h"

namespace Isetta {
class StopWatch {
  using HighResClock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<HighResClock>;

  TimePoint startTime;

 public:
  StopWatch() = default;
  StopWatch(const StopWatch& inStopWatch) = delete;
  StopWatch(StopWatch&& inStopWatch) = delete;

  StopWatch& operator=(const StopWatch& inStopWatch) = delete;
  StopWatch& operator=(StopWatch&& inStopWatch) = delete;

  void Start();
  float EvaluateInSecond() const;
  I64 EvaluateInNanoseconds() const;
  void Reset();
};
}  // namespace Isetta
