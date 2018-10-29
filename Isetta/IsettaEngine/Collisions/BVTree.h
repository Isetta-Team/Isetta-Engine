/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Collider.h"
#include <unordered_set>
#include "CollisionUtil.h"

namespace Isetta {

struct BVNode {
public:
  BVNode(AABB aabb) : aabb(aabb) {}
  BVNode(Collider* collider) : aabb(collider->GetAABB()), collider(collider) {}

  BVNode* parent{nullptr};
  BVNode* left{nullptr};
  BVNode* right{nullptr};

  AABB aabb;
  Collider* collider;

  bool IsLeaf() const {
    return left == nullptr;
  }
};

class BVTree {
public:
  BVTree() = default;
  ~BVTree();

  void Add(Collider* collider);
  void Remove(BVNode* node);
  void Update();
  void DebugDraw() const;

  RaycastHit RayCast(const Ray& ray);

  const CollisionUtil::ColliderPairSet& GetCollisionPairs();
  BVNode* root = nullptr;

private:
  std::vector<Collider*> colliders;
  CollisionUtil::ColliderPairSet colliderPairSet;
};

}  // namespace Isetta
