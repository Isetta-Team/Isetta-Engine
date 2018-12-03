/*
 * Copyright (c) 2018 Isetta
 */
#include "BVTree.h"

#include <queue>
#include "Collisions/RaycastHit.h"
#include "Core/Config/Config.h"
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Assert.h"
#include "Core/Debug/DebugDraw.h"
#include "Core/Geometry/Ray.h"
#include "Scene/Entity.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
void BVTree::Node::UpdateBranchAABB() {
  ASSERT(collider == nullptr && !IsLeaf());
  aabb = AABB::Encapsulate(left->aabb, right->aabb);
}

void BVTree::Node::UpdateLeafAABB() {
  ASSERT(IsLeaf() && collider != nullptr);
  aabb = collider->GetFatAABB();
}

void BVTree::Node::SwapOutChild(Node *const oldChild, Node *const newChild) {
  ASSERT(oldChild == left || oldChild == right);
  if (oldChild == left) {
    left = newChild;
    left->parent = this;
  } else {
    right = newChild;
    right->parent = this;
  }
}

BVTree::BVTree()
    : nodePool(CONFIG_VAL(collisionConfig.bvTreeNodeSize),
               CONFIG_VAL(memoryConfig.defaultPoolIncrement)) {}

void BVTree::AddCollider(Collider *const collider) {
  Node *newNode = nodePool.Get(collider);
  colNodeMap.insert({collider, newNode});
  AddNode(newNode);
}

void BVTree::RemoveCollider(Collider *const collider) {
  auto it = colNodeMap.find(collider);
  ASSERT(it != colNodeMap.end());
  RemoveNode(it->second, true);
  colNodeMap.erase(it);
}

void BVTree::Update() {
  BROFILER_CATEGORY("BVTree Update", Profiler::Color::Coral);
  Array<Node *> toReInsert;

  std::queue<Node *> q;
  if (root != nullptr) {
    q.push(root);
  }

  while (!q.empty()) {
    Node *cur = q.front();
    q.pop();

    if (cur->left != nullptr) q.push(cur->left);
    if (cur->right != nullptr) q.push(cur->right);

    if (cur->IsLeaf() && !cur->IsInFatAABB()) {
      toReInsert.PushBack(cur);
    }
  }

  for (auto node : toReInsert) {
    RemoveNode(node, false);
  }

  for (auto node : toReInsert) {
    node->UpdateLeafAABB();
    AddNode(node);
  }

#if _EDITOR
  DebugDraw();
#endif
}

bool BVTree::Raycast(const Ray &ray, RaycastHit *const hitInfo,
                     const float maxDistance) const {
  return Raycast(root, ray, hitInfo, maxDistance);
}
bool BVTree::Raycast(Node *const node, const Ray &ray,
                     RaycastHit *const hitInfo, const float maxDistance) const {
  if (node == nullptr || !node->aabb.Raycast(ray, nullptr, maxDistance)) {
    return false;
  }
  if (node->IsLeaf()) {
    RaycastHit hitTmp{};
    if (node->collider->Raycast(ray, &hitTmp, maxDistance) &&
        hitTmp.GetDistance() < hitInfo->GetDistance()) {
      *hitInfo = std::move(hitTmp);
      return true;
    }

    return false;
  }

  return Raycast(node->left, ray, hitInfo, maxDistance) ||
         Raycast(node->right, ray, hitInfo, maxDistance);
}

Array<RaycastHit> BVTree::RaycastAll(const Ray &ray, float maxDistance) const {
  Array<RaycastHit> hits;
  RaycastAll(root, &hits, ray, maxDistance);
  return hits;
}
void BVTree::RaycastAll(Node *const node, Array<RaycastHit> *hits, const Ray &ray,
                                     float maxDistance) const {
  if (node == nullptr || !node->aabb.Raycast(ray, nullptr, maxDistance)) {
    return;
  }
  if (node->IsLeaf()) {
    RaycastHit hitTmp{};
    if (node->collider->Raycast(ray, &hitTmp, maxDistance)) {
      hits->PushBack(std::move(hitTmp));
    }

    return;
  }

  RaycastAll(node->left, hits, ray, maxDistance);
  RaycastAll(node->right, hits, ray, maxDistance);
}

const CollisionUtil::ColliderPairSet &BVTree::GetCollisionPairs() {
  PROFILE
  colliderPairSet.clear();

  for (const auto &pair : colNodeMap) {
    if (pair.first->entity->isStatic) continue;

    Collider *collider = pair.first;
    AABB aabb = collider->GetFatAABB();
    std::queue<Node *> q;

    if (root != nullptr) {
      q.push(root);
    }

    while (!q.empty()) {
      Node *curNode = q.front();
      q.pop();

      if (curNode->IsLeaf()) {
        if (collider != curNode->collider) {
          colliderPairSet.insert({collider, curNode->collider});
        }
      } else {
        if (curNode->left->aabb.Intersect(aabb)) {
          q.push(curNode->left);
        }
        if (curNode->right->aabb.Intersect(aabb)) {
          q.push(curNode->right);
        }
      }
    }
  }

  return colliderPairSet;
}

Array<Collider *> BVTree::GetPossibleColliders(Collider *collider) const {
  PROFILE
  Array<Collider *> ret;

  AABB aabb = collider->GetFatAABB();
  std::queue<Node *> q;

  if (root != nullptr) {
    q.push(root);
  }

  while (!q.empty()) {
    Node *curNode = q.front();
    q.pop();

    if (curNode->IsLeaf()) {
      if (collider != curNode->collider) {
        ret.PushBack(curNode->collider);
      }
    } else {
      if (curNode->left->aabb.Intersect(aabb)) {
        q.push(curNode->left);
      }
      if (curNode->right->aabb.Intersect(aabb)) {
        q.push(curNode->right);
      }
    }
  }
  return ret;
}

void BVTree::AddNode(Node *const newNode) {
  PROFILE
  AABB newAABB = newNode->aabb;

  if (root == nullptr) {
    root = newNode;
    root->parent = nullptr;
  } else {
    Node *cur = root;

    while (!cur->IsLeaf()) {
      float leftIncrease =
          AABB::Encapsulate(cur->left->aabb, newAABB).SurfaceArea() -
          cur->left->aabb.SurfaceArea();

      float rightIncrease =
          AABB::Encapsulate(cur->right->aabb, newAABB).SurfaceArea() -
          cur->right->aabb.SurfaceArea();

      if (leftIncrease > rightIncrease) {
        cur = cur->right;
      } else {
        cur = cur->left;
      }
    }

    if (cur == root) {
      // cur is root
      root = nodePool.Get(AABB::Encapsulate(cur->aabb, newAABB));
      cur->parent = root;
      newNode->parent = root;
      root->left = cur;
      root->right = newNode;
    } else {
      // cur is actual leaf, convert cur to branch
      Node *newBranch =
          nodePool.Get(AABB::Encapsulate(cur->aabb, newNode->aabb));
      newBranch->parent = cur->parent;
      cur->parent->SwapOutChild(cur, newBranch);
      cur->parent = newBranch;
      newNode->parent = newBranch;
      newBranch->left = cur;
      newBranch->right = newNode;

      Node *parent = newBranch->parent;

      while (parent != nullptr) {
        parent->UpdateBranchAABB();
        parent = parent->parent;
      }
    }
  }
}

void BVTree::RemoveNode(Node *const node, const bool deleteNode) {
  PROFILE
  ASSERT(node->IsLeaf());

  if (node == root) {
    root = nullptr;
  } else if (node->parent == root) {
    Node *newRoot;

    if (node == root->left) {
      newRoot = root->right;
    } else {
      newRoot = root->left;
    }

    nodePool.Free(root);
    root = newRoot;
    root->parent = nullptr;
  } else {
    Node *parent = node->parent;
    Node *grandParent = parent->parent;

    ASSERT(grandParent != nullptr);
    ASSERT(node == parent->left || node == parent->right);

    if (node == parent->left) {
      grandParent->SwapOutChild(parent, parent->right);
    } else {
      grandParent->SwapOutChild(parent, parent->left);
    }

    nodePool.Free(parent);

    Node *cur = grandParent;
    while (cur != nullptr) {
      cur->UpdateBranchAABB();
      cur = cur->parent;
    }
  }

  if (deleteNode) {
    nodePool.Free(node);
  }
}

void BVTree::DebugDraw() const {
  if (!Config::Instance().drawConfig.bvtDrawAABBs.GetVal()) {
    return;
  }

  std::queue<Node *> q;

  if (root != nullptr) {
    q.push(root);
  }

  while (!q.empty()) {
    Node *cur = q.front();

    Color color;

    if (cur->IsLeaf()) {
#if _EDITOR
      // if (collisionSet.find(cur->collider) != collisionSet.end()) {
      color = Color::red;
      // } else {
      color = Color::green;
      // }
      DebugDraw::WireCube(Math::Matrix4::Translate(cur->aabb.GetCenter()) *
                              Math::Matrix4::Scale({cur->aabb.GetSize()}),
                          color, 1, .05);
#endif
    } else {
      int depth = 0;
      auto parent = cur->parent;
      while (parent != nullptr) {
        ++depth;
        parent = parent->parent;
      }
      color = Color::Lerp(Color::white, Color::black,
                          static_cast<float>(depth) / 10);
      DebugDraw::WireCube(Math::Matrix4::Translate(cur->aabb.GetCenter()) *
                              Math::Matrix4::Scale({cur->aabb.GetSize()}),
                          color, 1, .05);
    }

    q.pop();
    if (cur->left != nullptr) {
      q.push(cur->left);
    }
    if (cur->right != nullptr) {
      q.push(cur->right);
    }
  }
}
}  // namespace Isetta