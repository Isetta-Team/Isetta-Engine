/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Memory/MemoryManager.h"

namespace Isetta {
template <typename T>
class Array;

class Trie {
 public:
  struct Node {
    static const int ASCII_CHAR_SET = 128;
    struct Node* children[ASCII_CHAR_SET];
    bool isWord;

    Node();
    ~Node();

    Node(const Node& inNode);
    Node(Node&& inNode);
    Node& operator=(const Node& inNode);
    Node& operator=(Node&& inNode) noexcept;

   private:
    Node* Copy(const Node* const node);

    friend Trie;
  };

 private:
  Node* root{nullptr};
  int wordCnt{0}, depth{0};

  Node* NewNode();
  void GetWordsUtil(Node* const node, Array<std::string_view>* const words,
                    char* const word, int pos) const;

 public:
  Trie();
  ~Trie();

  Trie(const Trie& inTrie);
  Trie(Trie&& inTrie) noexcept;
  Trie& operator=(const Trie& inTrie);
  Trie& operator=(Trie&& inTrie);

  explicit Trie(const Array<std::string_view>& vec);

  void Insert(const std::string_view& key);
  Node* Find(const std::string_view& key);
  bool Contains(const std::string_view& key);
  bool HasPrefix(const std::string_view& key);
  Array<std::string_view> GetWords() const;
};

}  // namespace Isetta