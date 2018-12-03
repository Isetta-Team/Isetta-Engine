/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Application.h"
#include "Core/Config/CVar.h"

namespace Isetta {
class ISETTA_API EngineLoop {
 public:
  struct LoopConfig {
    CVar<int> maxFps{"max_fps", 16};
    CVar<int> maxSimCount{"max_simulation_count", 5};
  };

  // Start the whole game
  EngineLoop();
  ~EngineLoop();

  static EngineLoop& Instance();
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
  class CollisionsModule* collisionsModule;
  class CollisionSolverModule* collisionSolverModule;
  class Events* events;

  void Run();
  void StartUp();
  void Update();
  void FixedUpdate(float deltaTime) const;
  void VariableUpdate(float deltaTime) const;
  void ShutDown();

  void StartGameClock() const;

  friend class Application;
};
}  // namespace Isetta
