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

  void UpdateBranchAABB() {
    ASSERT(collider == nullptr && !IsLeaf());
    aabb = AABB::Encapsulate(left->aabb, right->aabb);
  }

  void UpdateLeafAABB() {
    ASSERT(IsLeaf() && collider != nullptr);
    aabb = collider->GetFatAABB();
  }

  void SwapOutChild(BVNode* oldChild, BVNode* newChild) {
    ASSERT(oldChild == left || oldChild == right);
    if (oldChild == left) {
      left = newChild;
      left->parent = this;
    } else {
      right = newChild;
      right->parent = this;
    }
  }

  bool IsLeaf() const { return left == nullptr; }
  bool IsInFatAABB() const { return aabb.Contains(collider->GetAABB()); }

  BVNode* parent{nullptr};
  BVNode* left{nullptr};
  BVNode* right{nullptr};

  Collider* const collider{nullptr};
  AABB aabb;
};

class BVTree {
 public:
  BVTree() = default;
  ~BVTree();

  void AddCollider(Collider* collider);
  void RemoveCollider(Collider* collider);
  void Update();

  RaycastHit RayCast(const Ray& ray);
  const CollisionUtil::ColliderPairSet& GetCollisionPairs();

 private:
  void AddNode(BVNode* newNode);
  void RemoveNode(BVNode* node, bool deleteNode);
  void DebugDraw() const;

  CollisionUtil::ColliderPairSet colliderPairSet;
  std::unordered_map<Collider*, BVNode*> colNodeMap;
  BVNode* root = nullptr;
};

}  // namespace Isetta
