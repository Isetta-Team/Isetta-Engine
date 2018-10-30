/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_set>
#include "Collider.h"
#include "CollisionUtil.h"
#include "Core/Debug/Assert.h"

namespace Isetta {

// BVNode serves two purposes: leaf and branch
struct BVNode {
 public:
  BVNode(AABB aabb) : aabb(aabb) {}
  BVNode(Collider* collider)
      : collider(collider), aabb(collider->GetFatAABB()) {}

  // TODO(YIDI): update code to use this
  void UpdateBranchAABB() {
    ASSERT(collider == nullptr && !IsLeaf());
    aabb = AABB::Encapsulate(left->GetAABB(), right->GetAABB());
  }

  void UpdateLeafAABB() {
    ASSERT(IsLeaf() && collider != nullptr);
    aabb = collider->GetFatAABB();
  }

  void SetAABB(const AABB& newAABB) { aabb = newAABB; }

  AABB GetAABB() const {
    return aabb;
  }

  bool IsLeaf() const { return left == nullptr; }

  bool IsInFatAABB() const { return aabb.Contains(collider->GetAABB()); }

  BVNode* parent{nullptr};
  BVNode* left{nullptr};
  BVNode* right{nullptr};

  Collider* collider{nullptr};
  AABB aabb;
};

class BVTree {
 public:
  BVTree() = default;
  ~BVTree();

  void AddCollider(Collider* collider);
  void Remove(BVNode* node, bool deleteNode);
  void Update();

  RaycastHit RayCast(const Ray& ray);

  const CollisionUtil::ColliderPairSet& GetCollisionPairs();
  BVNode* root = nullptr;

 private:
  void AddNode(BVNode* newNode);
  void DebugDraw() const;

  std::vector<Collider*> colliders;
  CollisionUtil::ColliderPairSet colliderPairSet;
};

}  // namespace Isetta
