/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {

class ModuleManager {
 public:
  ModuleManager();
  ~ModuleManager();

  /**
   * \brief Start all submodules
   */
  void StartUp();
  /**
   * \brief Run simulation in update phase
   * \param deltaTime Update delta time
   */
  void SimulationUpdate(float deltaTime);
  /**
   * \brief Run render in update phase
   * \param deltaTime Update delta time
   */
  void RenderUpdate(float deltaTime);
  /**
   * \brief Shutdown all submodules
   */
  void ShutDown();

 private:
  class MemoryManager* memoryManager;
  class AudioModule* audioModule;
  class WindowModule* windowModule;
  class RenderModule* renderModule;
  class InputModule* inputModule;
  class NetworkingModule* networkingModule;
};
}  // namespace Isetta
