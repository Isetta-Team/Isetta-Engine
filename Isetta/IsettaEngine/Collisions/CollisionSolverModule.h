/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Scene/Layers.h"
#include "Collisions/CollisionUtil.h"

namespace Isetta {
class CollisionSolverModule {
 public: 
 private:
  CollisionSolverModule() = default;
  ~CollisionSolverModule() = default;

  Math::Vector3 Solve(Collider* collider, Math::Vector3 point);

  void StartUp();
  void Update();
  void ShutDown();

  static class CollisionsModule* collisionsModule;

  friend class EngineLoop;
  friend class CollisionsModule;
};
}  // namespace Isetta
