/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {
template <typename T>
struct AVLNode {
  T data;
  AVLNode *left, *right;

  AVLNode(T val) {
    data = nullptr;
    left = right = nullptr;
  }

  // inline bool Compare(const AVLNode other)
};

template <typename T>
class AVLTree {
 private:
  AVLNode<T>* root;
  /** @brief Returns <= 0 if first value is less than second, > 0 otherwise
   *
   */
  Func<bool, T, T> compare;

 public:
  template <typename T>
  AVLTree(const Func<bool, T, T>& comp) : compare{comp} {
    root = nullptr;
  }

  template <typename T>
  inline int Height(AVLNode<T>* node) {
    int height = 0;
    if (node) {
      int left = Height(node->left);
      int right = Height(node->right);
      height = Math::Util::Max(left, right) + 1;
    }
    return height;
  }
  template <typename T>
  inline int BranchDiff(AVLNode<T>* node) {
    int left = Height(node->left);
    int right = Height(node->right);
    return left - right;
  }

  template <typename T>
  inline AVLNode<T>* RotationRR(AVLNode<T>* parent) {
    AVLNode<T>* tmp = parent->right;
    parent->right = tmp->left;
    tmp->left = parent;
    return tmp;
  }
  template <typename T>
  inline AVLNode<T>* RotationRL(AVLNode<T>* parent) {
    AVLNode<T>* tmp = parent->right;
    parent->right = RotationLL(tmp);
    return RotationRR(parent);
  }
  template <typename T>
  inline AVLNode<T>* RotationLR(AVLNode<T>* parent) {
    AVLNode<T>* tmp = parent->left;
    parent->left = RotationRR(tmp);
    return RotationLL(parent);
  }
  template <typename T>
  inline AVLNode<T>* RotationLL(AVLNode<T>* parent) {
    AVLNode<T>* tmp = parent->left;
    parent->left = tmp->right;
    tmp->right = parent;
    return tmp;
  }

  template <typename T>
  inline AVLNode<T>* Balance(AVLNode<T>* node) {
    int bal = BranchDiff(node);
    if (bal > 1) {
      if (BranchDiff(node->left) > 0) {
        node = RotationLL(node);
      } else {
        node = RotationLR(node);
      }
    } else if (bal < -1) {
      if (BranchDiff(node->right) > 0) {
        node = RotationRL(node);
      } else {
        node = RotationRR(node);
      }
    }
    return node;
  }
  template <typename T>
  inline AVLNode<T>* Insert(AVLNode<T>* node, T data) {
    if (node == nullptr) {
      node = new AVLNode<T>(data);
    } else if (compare(data, node->data)) {
      node->left = Insert(node->left, data);
      node = Balance(node);
    } else {
      node->right = Insert(node->right, data);
      node = Balance(node);
    }
    return node;
  }
  template <typename T>
  inline AVLNode<T>* Insert(T data) {
    if (root == nullptr) {
      root = new AVLNode<T>(data);
    } else if (compare(data, root->data)) {
      root->left = Insert(root->left, data);
      root = Balance(root);
    } else {
      root->right = Insert(root->right, data);
      root = Balance(root);
    }
    return root;
  }
};
}  // namespace Isetta
