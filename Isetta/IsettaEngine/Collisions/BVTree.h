/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <unordered_set>
#include "AABB.h"
#include "Collider.h"
#include "CollisionUtil.h"

namespace Isetta {

class BVTree {
 private:
  // BVNode serves two purposes: leaf and branch
  struct Node {
    explicit Node(AABB aabb) : aabb(std::move(aabb)) {}
    explicit Node(class Collider* const collider)
        : collider(collider), aabb(collider->GetFatAABB()) {}

    void UpdateBranchAABB();
    void UpdateLeafAABB();
    void SwapOutChild(Node* const oldChild, Node* const newChild);

    inline bool IsLeaf() const { return left == nullptr; }
    inline bool IsInFatAABB() const {
      return aabb.Contains(collider->GetAABB());
    }

    Node* parent{nullptr};
    Node* left{nullptr};
    Node* right{nullptr};

    class Collider* collider{nullptr};
    AABB aabb;
  };

  BVTree() = default;
  friend class CollisionsModule;

 public:
  ~BVTree();

  void AddCollider(class Collider* const collider);
  void RemoveCollider(class Collider* const collider);
  void Update();

  bool Raycast(const class Ray& ray, class RaycastHit* const hitInfo,
               float maxDistance);
  bool Raycast(Node* const node, const class Ray& ray,
               class RaycastHit* const hitInfo, float maxDistance);
  const CollisionUtil::ColliderPairSet& GetCollisionPairs();

 private:
  void AddNode(Node* const newNode);
  void RemoveNode(Node* const node, bool deleteNode);
  void DebugDraw() const;

  CollisionUtil::ColliderPairSet colliderPairSet;
  std::unordered_map<class Collider*, Node*> colNodeMap;
  Node* root = nullptr;
};

}  // namespace Isetta
