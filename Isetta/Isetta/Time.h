#pragma once
#include <chrono>

using namespace std;

namespace Time {
	chrono::time_point<chrono::high_resolution_clock> startTime;
	float time;
	float deltaTime;
	int frameCount;
}