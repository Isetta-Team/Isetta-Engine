#pragma once
#include "Core/Config/Config.h"
#include "Core/Time/Clock.h"
#include "EngineLoop.h"

namespace Isetta {
class Time {
 public:
  /**
   * @brief Get the Elapsed Time of engine
   *
   * @return double
   */
  static double GetElapsedTime() {
    return EngineLoop::GetGameClock().GetElapsedTime();
  }
  /**
   * @brief Get the Delta Time since last update frame
   *
   * @return double
   */
  static double GetDeltaTime() {
    return EngineLoop::GetGameClock().GetDeltaTime();
  }
  /**
   * @brief Get the Fixed Delta Time
   *
   * @return double
   */
  static double GetFixedDeltaTime() {
    static double fixedDeltaTime = 1.0 / CONFIG_VAL(loopConfig.maxFps);
    return fixedDeltaTime;
  }
  /**
   * @brief Get the Elapsed Unscaled Time
   *
   * @return double
   */
  static double GetElapsedUnscaledTime() {
    return EngineLoop::GetGameClock().GetElapsedUnscaledTime();
  }
  /**
   * @brief Get the Frame Count
   *
   * @return U64
   */
  static U64 GetFrameCount() {
    return EngineLoop::GetGameClock().GetTimeFrame();
  }
  /**
   * @brief Get the Timestamp
   *
   * @return U64
   */
  static U64 GetTimestamp() {
    return EngineLoop::GetGameClock().GetTimestamp();
  }
  /**
   * @brief Set the Game Scale
   *
   * @param newTimeScale scale to set
   */
  static void SetGameScale(const float newTimeScale) {
    EngineLoop::GetGameClock().timeScale = newTimeScale;
  }
  /**
   * @brief Get the Time Scale
   *
   * @return float
   */
  static float GetTimeScale() { return EngineLoop::GetGameClock().timeScale; }
  /**
   * @brief Pause the engine update
   * [WARNING]: Pauses entire engine, updates will halt
   *
   */
  static void Pause() { EngineLoop::GetGameClock().isPause = true; }
  /**
   * @brief Resumes the engine update
   *
   */
  static void Resume() { EngineLoop::GetGameClock().isPause = false; }
};
}  // namespace Isetta