#pragma once
#include "Core/Time/Clock.h"
#include "EngineLoop.h"
#include "Core/Config/Config.h"

namespace Isetta {
class Time {
 public:
  static double GetElapsedTime() {
    return EngineLoop::GetGameClock().GetElapsedTime();
  }
  static double GetDeltaTime() {
    return EngineLoop::GetGameClock().GetDeltaTime();
  }
  static double GetFixedDeltaTime() {
    static double fixedDeltaTime = 1.0 / CONFIG_VAL(loopConfig.maxFps);
    return fixedDeltaTime;
  }
  static double GetElapsedUnscaledTime() {
    return EngineLoop::GetGameClock().GetElapsedUnscaledTime();
  }
  static U64 GetTimeFrame() {
    return EngineLoop::GetGameClock().GetTimeFrame();
  }
  static U64 GetTimestamp() {
    return EngineLoop::GetGameClock().GetTimestamp();
  }
  static void SetGameScale(const float newTimeScale) {
    EngineLoop::GetGameClock().timeScale = newTimeScale;
  }
  static float GetTimeScale() { return EngineLoop::GetGameClock().timeScale; }
  static void Pause() { EngineLoop::GetGameClock().isPause = true; }
  static void Resume() { EngineLoop::GetGameClock().isPause = false; }
};
}  // namespace Isetta