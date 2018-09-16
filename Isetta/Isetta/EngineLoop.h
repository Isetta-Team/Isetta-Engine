/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/ModuleManager.h"

namespace Isetta {

class EngineLoop {
 private:
  bool isGameRunning;
  double accumulateTime;
  double intervalTime;
  int maxSimulationCount;

  ModuleManager moduleManager;

  void StartUp();
  void Update();
  void ShutDown();

 public:
  EngineLoop() = default;
  ~EngineLoop() = default;

  void Run();

  static class Clock& GetGameClock();
};
}  // namespace Isetta