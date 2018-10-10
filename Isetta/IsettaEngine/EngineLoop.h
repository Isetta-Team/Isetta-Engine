/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#ifdef _DLL
#ifdef ISETTA_EXPORTS
#define ISETTA_API __declspec(dllexport)
#else
#define ISETTA_API __declspec(dllimport)
#endif
#endif

#include "Core/Config/CVar.h"

namespace Isetta {
class EngineLoop {
 public:
  struct LoopConfig {
    CVar<int> maxFps{"max_fps", 16};
    CVar<int> maxSimCount{"max_simulation_count", 5};
  };

  ISETTA_API EngineLoop();
  ISETTA_API ~EngineLoop();

  ISETTA_API void Run();

  ISETTA_API static class Clock& GetGameClock();

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
