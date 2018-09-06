/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {

class ModuleManager {
 public:
  ModuleManager() = default;

  void StartUp();
  void Update();
  void ShutDown();

 private:
  class AudioModule* audioModule;
  class RenderModule* renderModule;
  class InputModule* inputModule;
};
}  // namespace Isetta