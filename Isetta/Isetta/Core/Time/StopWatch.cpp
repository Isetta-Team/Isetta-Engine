/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Time/StopWatch.h"

namespace Isetta {

void StopWatch::Start() { startTime = HighResClock::now(); }

float StopWatch::EvaluateInSecond() const {
  return (HighResClock::now() - startTime).count() / 1000.f;
}

I64 StopWatch::EvaluateInMilliseconds() const {
  return (HighResClock::now() - startTime).count();
}

void StopWatch::Reset() { startTime = HighResClock::now(); }
}  // namespace Isetta
