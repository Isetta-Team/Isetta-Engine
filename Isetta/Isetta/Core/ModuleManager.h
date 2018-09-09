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
  class WindowModule* windowModule;
};
}  // namespace Isetta