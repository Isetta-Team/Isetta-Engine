#include "BVTree.h"
#include <queue>
#include <unordered_set>
#include "Core/Debug/DebugDraw.h"
#include "Core/Time/Time.h"
#include "Util.h"

namespace Isetta {

BVTree::~BVTree() {
  std::queue<BVNode*> q;
  q.push(root);
  while (!q.empty()) {
    BVNode* cur = q.front();
    q.pop();
    if (cur != nullptr) {
      q.push(cur->left);
      q.push(cur->right);
      delete cur;
    }
  }
}

void BVTree::Add(Collider* collider) {
  colliders.push_back(collider);

  BVNode* newNode = new BVNode{collider};
  AABB& newAABB = newNode->aabb;

  if (root == nullptr) {
    root = newNode;
  } else {
    BVNode* cur = root;

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

    if (cur->parent == nullptr) {
      // cur is root
      root = new BVNode(AABB::Encapsulate(cur->aabb, newAABB));
      cur->parent = root;
      newNode->parent = root;
      root->left = cur;
      root->right = newNode;
    } else {
      // cur is actual leaf, convert cur to branch
      BVNode* newLeaf = new BVNode{cur->collider};
      cur->left = newLeaf;
      cur->right = newNode;
      cur->aabb = AABB::Encapsulate(newLeaf->aabb, newNode->aabb);
      cur->collider = nullptr;
      newLeaf->parent = cur;
      newNode->parent = cur;
      BVNode* parent = cur->parent;

      while (parent != nullptr) {
        parent->aabb = AABB::Encapsulate(parent->aabb, cur->aabb);
        cur = parent;
        parent = cur->parent;
      }
    }
  }
}

void BVTree::DebugDraw() const {
  std::queue<BVNode*> q;

  if (root != nullptr) {
    q.push(root);
  }

  while (!q.empty()) {
    BVNode* cur = q.front();

    Color color;
    if (cur->IsLeaf()) {
      color = Color::green;
    } else {
      int depth = 0;
      auto parent = cur->parent;
      while (parent != nullptr) {
        depth++;
        parent = parent->parent;
      }
      color = Color::Lerp(Color::white, Color::black,
                          static_cast<float>(depth) / 10);
      DebugDraw::WireCube(Math::Matrix4::Translate(cur->aabb.GetCenter()) *
                              Math::Matrix4::Scale({cur->aabb.GetSize()}),
                          color, 1, .1);
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

const CollisionUtil::ColliderPairSet& BVTree::GetCollisionPairs() {
  colliderPairSet.clear();

  for (const auto& curCollider : colliders) {
    AABB aabb = curCollider->GetAABB();
    std::queue<BVNode*> q;

    if (root != nullptr) {
      q.push(root);
    }

    while (!q.empty()) {
      auto curNode = q.front();
      q.pop();

      if (curNode->IsLeaf()) {
        if (curCollider != curNode->collider) {
          colliderPairSet.insert({curCollider, curNode->collider});
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

}  // namespace Isetta