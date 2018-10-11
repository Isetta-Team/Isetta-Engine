/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

namespace Isetta {
class IModule {
 public:
  virtual ~IModule() {}
  virtual void StartUp() = 0;
  virtual void Update() {}
  virtual void ShutDown() = 0;
};
}  // namespace Isetta
