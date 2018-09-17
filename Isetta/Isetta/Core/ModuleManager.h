/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {

class ModuleManager {
 public:
  ModuleManager();
  ~ModuleManager();

  void StartUp();
  void Update(float deltaTime);
  void ShutDown();
  class NetworkingModule* networkingModule;

 private:
  class AudioModule* audioModule;
  class WindowModule* windowModule;
  class RenderModule* renderModule;
  class InputModule* inputModule;
};
}  // namespace Isetta
