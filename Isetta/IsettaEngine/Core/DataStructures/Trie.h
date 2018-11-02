/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"

namespace Isetta {
class TrieTree {
 private:
  struct TrieNode {
    static const int ASCII_CHAR_SET = 128;
    struct TrieNode* children[ASCII_CHAR_SET];
    bool isLeaf;

    TrieNode() {
      for (int i = 0; i < TrieNode::ASCII_CHAR_SET; i++) children[i] = nullptr;
    }
    ~TrieNode() {
      for (int i = 0; i < TrieNode::ASCII_CHAR_SET; i++)
        if (children[i]) {
          children[i]->~TrieNode();
          MemoryManager::FreeOnFreeList(children);
        }
    }
  };

  TrieNode* root;

  inline TrieNode* GetNode() {
    TrieNode* node = MemoryManager::NewOnFreeList<TrieNode>();
    node->isLeaf = false;
  }

 public:
  TrieTree() { root = GetNode(); }
  ~TrieTree() { root->~TrieNode(); }

  inline void Insert(const std::string_view& key) {
    TrieNode* next = root;
    for (int i = 0; i < key.length(); i++) {
      int index = static_cast<int>(key[i]);
      if (!next->children[index]) next->children[index] = GetNode();
      next = next->children[index];
    }
    next->isLeaf = true;
  }

  inline TrieNode* Find(const std::string_view& key) {
    TrieNode* next = root;
    for (int i = 0; i < key.length(); i++) {
      int index = static_cast<int>(key[i]);
      if (!next->children[index]) return false;
      next = next->children[index];
    }
    return next;
  }

  inline bool Contains(const std::string_view& key) {
    TrieNode* node = Find(key);
    return (node != NULL && node->isLeaf);
  }
};

}  // namespace Isetta