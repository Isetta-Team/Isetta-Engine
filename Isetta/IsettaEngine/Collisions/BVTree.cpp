#include "BVTree.h"
#include <queue>
#include <unordered_set>
#include "Core/Debug/DebugDraw.h"
#include "Core/Time/Time.h"
#include "Util.h"
#include "Scene/Entity.h"

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

void BVTree::AddCollider(Collider* collider) {
  colliders.push_back(collider);
  AddNode(new BVNode{collider});
}

void BVTree::Remove(BVNode* node, bool deleteNode) {
  ASSERT(node->IsLeaf());

  if (node == root) {
    root = nullptr;
  } else if (node->parent == root) {
    BVNode* newRoot;

    if (node == root->left) {
      newRoot = root->right;
    } else {
      newRoot = root->left;
    }

    delete root;
    root = newRoot;
    root->parent = nullptr;
  } else {
    BVNode* parent = node->parent;
    BVNode* grandParent = parent->parent;

    ASSERT(grandParent != nullptr);
    ASSERT(node == parent->left || node == parent->right);

    if (node == parent->left) {
      grandParent->SwapOutChild(parent, parent->right);
    } else {
      grandParent->SwapOutChild(parent, parent->left);
    }

    BVNode* cur = grandParent;
    while (cur != nullptr) {
      cur->UpdateBranchAABB();
      cur = cur->parent;
    }
  }

  if (deleteNode) {
    delete node;
  }
}

void BVTree::Update() {
  std::vector<BVNode*> toReInsert;

  std::queue<BVNode*> q;
  if (root != nullptr) {
    q.push(root);  
  }
  
  while (!q.empty()) {
    BVNode* cur = q.front();
    q.pop();

    if (cur->left != nullptr) q.push(cur->left);
    if (cur->right != nullptr) q.push(cur->right);

    if (cur->IsLeaf() && !cur->IsInFatAABB()) {
      toReInsert.push_back(cur);
    }
  }

  for (auto node : toReInsert) {
    Remove(node, false);
    LOG_INFO(Debug::Channel::General, "Removing %s", node->collider->GetEntity()->GetName().c_str());
  }

  for (auto node : toReInsert) {
    node->UpdateLeafAABB();
    AddNode(node);
    LOG_INFO(Debug::Channel::General, "Adding %s", node->collider->GetEntity()->GetName().c_str());
  }

  DebugDraw();
}

void BVTree::AddNode(BVNode* newNode) {
  AABB newAABB = newNode->aabb;

  if (root == nullptr) {
    root = newNode;
    root->parent = nullptr;
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

    if (cur == root) {
      // cur is root
      root = new BVNode(AABB::Encapsulate(cur->aabb, newAABB));
      cur->parent = root;
      newNode->parent = root;
      root->left = cur;
      root->right = newNode;
    } else {
      // TODO(YIDI): Sometimes parent is delete but still referenced

      // cur is actual leaf, convert cur to branch
      BVNode* newLeaf = new BVNode{cur->collider};
      cur->collider = nullptr;
      cur->left = newLeaf;
      cur->right = newNode;
      newLeaf->parent = cur;
      newNode->parent = cur;

      cur->UpdateBranchAABB();
      BVNode* parent = cur->parent;

      while (parent != nullptr) {
        parent->UpdateBranchAABB();
        parent = parent->parent;
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
      DebugDraw::WireCube(Math::Matrix4::Translate(cur->aabb.GetCenter()) *
                              Math::Matrix4::Scale({cur->aabb.GetSize()}),
                          color, 1, .05);
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

const CollisionUtil::ColliderPairSet& BVTree::GetCollisionPairs() {
  colliderPairSet.clear();

  for (const auto& curCollider : colliders) {
    AABB aabb = curCollider->GetFatAABB();
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