/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

class IModule {
 public:
  virtual ~IModule() {}
  virtual void StartUp() = 0;
  virtual void Update();
  virtual void ShutDown() = 0;
};