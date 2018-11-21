/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include "Collisions/CollisionUtil.h"
#include "Scene/Layers.h"

namespace Isetta {
class CollisionSolverModule {
 public:
  struct Collision {
    Math::Vector3 hitPoint = Math::Vector3::zero;
    Math::Vector3 pushDir = Math::Vector3::forward;
    bool onEdge = false;
  };

 private:
  CollisionSolverModule() = default;
  ~CollisionSolverModule() = default;

  Collision Solve(Collider* collider, Math::Vector3 point);
  Math::Vector3 Resolve(Collider* collider1, Collision collision1,
                        Collider* collider2, Collision collision2);

  void StartUp();
  void Update();
  void ShutDown();

  const float EPS = 1e-6;

  static class CollisionsModule* collisionsModule;

  friend class EngineLoop;
  friend class CollisionsModule;
  friend class StackAllocator;
};
}  // namespace Isetta
