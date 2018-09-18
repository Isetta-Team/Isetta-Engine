/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Config/CVar.h"
#include "Core/ModuleManager.h"

namespace Isetta {

class EngineLoop {
 public:
  struct LoopConfig {
    CVarInt maxFps = {"max_fps", 16};
    CVarInt maxSimCount = {"max_simulation_count", 5};
  };

 private:
  bool isGameRunning;
  double accumulateTime;
  double intervalTime;
  int maxSimulationCount;

  ModuleManager moduleManager;

  void StartUp();
  void Update();
  void ShutDown();

  void StartGameClock() const;

 public:
  EngineLoop() = default;
  ~EngineLoop() = default;

  void Run();

  static class Clock& GetGameClock();
};
}  // namespace Isetta