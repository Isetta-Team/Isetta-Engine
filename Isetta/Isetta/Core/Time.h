/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <chrono>

namespace Time {
std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
float time;
float deltaTime;
int frameCount;
}  // namespace Time
