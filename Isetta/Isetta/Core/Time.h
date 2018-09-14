/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <chrono>

namespace Time {
  extern std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  extern float time;
  extern float deltaTime;
  extern int frameCount;
}  // namespace Time
