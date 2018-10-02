/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {
/*
 * Requirements:
 * 1. Alloc: Go through the sorted (by address) list and find the first one that
 * fits size need
 * 2. Free: Add the pointer back to the list, and try to merge adjacent ones.
 * It's easy to find the next Node. Merging with last node can be done during
 * insertion
 * 3. Alignment requirement
 */
class FreeListAllocator {
 public:
  // This class is using RAII
  explicit FreeListAllocator(Size size);
  ~FreeListAllocator();
  // FreeListAllocator(void* headPtr, Size size);

  void* Alloc(Size size, U8 alignment = 16);
  void Free(void* memPtr);

 private:
  struct Node {
    Size size;
    Node* next;
    Node(const Size size) : size(size), next(nullptr) {}
  };

  struct AllocHeader {
    Size size;
    // for alignment. U8 is big enough
    U64 adjustment;
    AllocHeader(const Size size, const U64 adjustment)
        : size(size), adjustment(adjustment) {}
  };

  void RemoveNode(Node* node);
  static void InsertNodeAt(Node* pos, Node* newNode);
  static void TryMergeWithNext(Node* node);

  // both are sitting on memHead
  Node* head;
  void* memHead;

  const Size nodeSize = sizeof(Node);
  const Size headerSize = sizeof(AllocHeader);
};

}  // namespace Isetta
