/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Config/CVar.h"

namespace Isetta {

class EngineLoop {
 public:
  struct LoopConfig {
    CVar<int> maxFps{"max_fps", 16};
    CVar<int> maxSimCount{"max_simulation_count", 5};
  };

  EngineLoop();
  ~EngineLoop();

  void Run();

  static class Clock& GetGameClock();

 private:
  bool isGameRunning;
  double accumulateTime;
  double intervalTime;
  int maxSimulationCount;

  class MemoryManager* memoryManager;
  class AudioModule* audioModule;
  class WindowModule* windowModule;
  class RenderModule* renderModule;
  class InputModule* inputModule;
  class GUIModule* guiModule;
  class NetworkingModule* networkingModule;

  void StartUp();
  void Update();
  void FixedUpdate(float deltaTime);
  void VariableUpdate(float deltaTime);
  void ShutDown();

  void StartGameClock() const;

  class TestInitalization;
  friend class TestInitialization;
};
}  // namespace Isetta