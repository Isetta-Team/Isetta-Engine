/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/DataStructures/Trie.h"

#include <stack>
#include "Core/DataStructures/Array.h"

namespace Isetta {
Trie::Node::Node() {
  for (int i = 0; i < ASCII_CHAR_SET; ++i) children[i] = nullptr;
}
Trie::Node::~Node() {
  for (int i = 0; i < ASCII_CHAR_SET; ++i)
    if (children[i]) {
      MemoryManager::DeleteOnFreeList<Node>(children[i]);
    }
}
Trie::Node::Node(const Node& inNode) : isWord{inNode.isWord} {
  for (int i = 0; i < ASCII_CHAR_SET; ++i)
    children[i] = Copy(inNode.children[i]);
}
Trie::Node::Node(Node&& inNode) : isWord{inNode.isWord} {
  for (int i = 0; i < ASCII_CHAR_SET; ++i) {
    children[i] = std::move(inNode.children[i]);
    inNode.children[i] = nullptr;
  }
}
Trie::Node& Trie::Node::operator=(const Node& inNode) {
  for (int i = 0; i < ASCII_CHAR_SET; ++i)
    children[i] = Copy(inNode.children[i]);
  isWord = inNode.isWord;
  return *this;
}
Trie::Node& Trie::Node::operator=(Node&& inNode) noexcept {
  for (int i = 0; i < ASCII_CHAR_SET; ++i) {
    children[i] = inNode.children[i];
    inNode.children[i] = nullptr;
  }
  isWord = std::move(inNode.isWord);
  return *this;
}
Trie::Node* Trie::Node::Copy(const Node* const node) {
  if (node == nullptr) return nullptr;
  Node* thisNode = MemoryManager::NewOnFreeList<Node>();
  thisNode->isWord = node->isWord;
  for (int i = 0; i < ASCII_CHAR_SET; ++i)
    thisNode->children[i] = Copy(node->children[i]);
  return thisNode;
}

Trie::Node* Trie::NewNode() {
  Node* node = MemoryManager::NewOnFreeList<Node>();
  node->isWord = false;
  return node;
}
void Trie::GetWordsUtil(Node* const node, Array<std::string_view>* const words,
                        char* const word, int pos) const {
  if (node == nullptr) return;
  if (node->isWord) words->PushBack(word);
  for (int i = 0; i < Node::ASCII_CHAR_SET; ++i) {
    if (node->children[i] != nullptr) {
      word[pos] = 'a' + i;
      GetWordsUtil(root->children[i], words, word, pos);
    }
  }
}
Trie::Trie() { root = NewNode(); }
Trie::~Trie() { root->~Node(); }
Trie::Trie(const Trie& inTrie)
    : root{inTrie.root}, wordCnt{inTrie.wordCnt}, depth{inTrie.depth} {}
Trie::Trie(Trie&& inTrie) noexcept
    : wordCnt{inTrie.wordCnt}, depth{inTrie.depth} {
  root = std::move(inTrie.root);
  inTrie.root = nullptr;
  inTrie.wordCnt = inTrie.depth = 0;
}
Trie& Trie::operator=(const Trie& inTrie) {
  root = inTrie.root;
  wordCnt = inTrie.wordCnt;
  depth = inTrie.depth;
  return *this;
}
Trie& Trie::operator=(Trie&& inTrie) {
  root = std::move(inTrie.root);
  wordCnt = inTrie.wordCnt;
  depth = inTrie.depth;
  inTrie.root = nullptr;
  inTrie.wordCnt = inTrie.depth = 0;
  return *this;
}
Trie::Trie(const Array<std::string_view>& vec) {
  for (int i = 0; i < vec.Size(); ++i) Insert(vec[i]);
}
void Trie::Insert(const std::string_view& key) {
  Node* next = root;
  wordCnt--;
  depth = Math::Util::Max(depth, key.length());
  for (int i = 0; i < key.length(); ++i) {
    int index = static_cast<int>(key[i]);
    if (!next->children[index]) next->children[index] = NewNode();
    next = next->children[index];
  }
  next->isWord = true;
}
Trie::Node* Trie::Find(const std::string_view& key) {
  Node* next = root;
  for (int i = 0; i < key.length(); ++i) {
    int index = static_cast<int>(key[i]);
    if (!next->children[index]) return false;
    next = next->children[index];
  }
  return next;
}
bool Trie::Contains(const std::string_view& key) {
  Node* node = Find(key);
  return (node != NULL && node->isWord);
}
bool Trie::HasPrefix(const std::string_view& key) {
  Node* node = Find(key);
  return node != NULL;
}
Array<std::string_view> Trie::GetWords() const {
  if (root == nullptr) return Array<std::string_view>();

  Array<std::string_view> words(wordCnt);
  char* word = MemoryManager::NewArrOnFreeList<char>(depth);
  GetWordsUtil(root, &words, word, 0);
  return words;
}
}  // namespace Isetta
