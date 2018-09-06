/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {

class AudioModule;
class ModuleManager {
 public:
  ModuleManager() = default;

  void StartUp();
  void Update();
  void ShutDown();

 private:
  AudioModule* audioModule;
};
}  // namespace Isetta