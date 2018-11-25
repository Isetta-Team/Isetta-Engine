/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <set>
#include <unordered_map>
#include "AABB.h"
#include "Collider.h"
#include "CollisionUtil.h"
#include "Core/Memory/TemplatePoolAllocator.h"

namespace Isetta::Math {
class Ray;
}

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
    void SwapOutChild(Node* oldChild, Node* newChild);

    bool IsLeaf() const { return left == nullptr; }
    bool IsInFatAABB() const { return aabb.Contains(collider->GetAABB()); }

    class Collider* collider{nullptr};
    AABB aabb;

    Node* parent{nullptr};
    Node* left{nullptr};
    Node* right{nullptr};
  };

  BVTree();
  friend class CollisionsModule;
  friend class CollisionSolverModule;
  friend class FreeListAllocator;

 public:
  ~BVTree() = default;

  void AddCollider(class Collider* collider);
  void RemoveCollider(class Collider* collider);
  void Update();

  bool Raycast(const Ray& ray, class RaycastHit* hitInfo,
               float maxDistance) const;
  bool Raycast(Node* node, const Ray& ray, class RaycastHit* hitInfo,
               float maxDistance) const;
  Array<RaycastHit> RaycastAll(const Ray& ray, float maxDistance) const;
  void RaycastAll(Node* node, Array<RaycastHit>* hits, const Ray& ray,
                               float maxDistance) const;

  const CollisionUtil::ColliderPairSet& GetCollisionPairs();
  Array<Collider*> GetPossibleColliders(class Collider* collider) const;

 private:
  void AddNode(Node* newNode);
  void RemoveNode(Node* node, bool deleteNode);
  void DebugDraw() const;

  CollisionUtil::ColliderPairSet colliderPairSet;
  std::unordered_map<class Collider*, Node*> colNodeMap;
  Node* root = nullptr;
  TemplatePoolAllocator<Node> nodePool;
#if _EDITOR
  // std::set<class Collider*> collisionSet;
#endif
};

}  // namespace Isetta
