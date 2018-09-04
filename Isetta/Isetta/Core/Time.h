// "Copyright [2018] Isetta"
#pragma once
#ifndef ISETTA_ISETTA_CORE_TIME_H_
#define ISETTA_ISETTA_CORE_TIME_H_
#include <chrono>

namespace Time {
std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
float time;
float deltaTime;
int frameCount;
}  // namespace Time

#endif  // ISETTA_ISETTA_CORE_TIME_H_
