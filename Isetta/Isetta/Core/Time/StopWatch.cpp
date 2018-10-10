/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Time/StopWatch.h"

namespace Isetta {

void StopWatch::Start() { startTime = HighResClock::now(); }

float StopWatch::EvaluateInSecond() const {
  return Seconds(HighResClock::now() - startTime).count();
}

I64 StopWatch::EvaluateInNanoseconds() const {
  return Nanoseconds(HighResClock::now() - startTime).count();
}

void StopWatch::Reset() { startTime = HighResClock::now(); }
}  // namespace Isetta
