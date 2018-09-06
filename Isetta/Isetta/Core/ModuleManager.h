/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
class ModuleManager {
 public:
  ModuleManager() {}

  void StartUp();
  void Update();
  void ShutDown();
};
}  // namespace Isetta