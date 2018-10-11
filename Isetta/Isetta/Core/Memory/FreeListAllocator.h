/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "MemUtil.h"

namespace Isetta {
/*
 * Requirements:
 * 1. Alloc: Go through the sorted (by address) list and find the first one that
 * fits size need
 * 2. Free: Add the pointer back to the list, and try to merge adjacent ones.
 * It's easy to find the next Node. Merging with last node can be done during
 * insertion
 * 3. Satisfy alignment requirement
 */
// TODO(YIDI): Add expansion functionality
class FreeListAllocator {
 public:
  // This class is using RAII
  FreeListAllocator() = default;
  explicit FreeListAllocator(Size size);
  FreeListAllocator(void* headPtr, Size size, Action<void*> freeCallback);
  ~FreeListAllocator() = default;

  void* Alloc(Size size, U8 alignment);
  void Free(void* memPtr);

  template <typename T, typename... args>
  T* New(args... argList);

  template <typename T>
  T* NewArr(Size length, const U8 alignment);

 private:
  struct Node {
    Size size;
    Node* next;
    Node(const Size size) : size(size), next(nullptr) {}
  };

  struct AllocHeader {
    Size size;
    // using U64 for alignment. U8 is big enough
    U64 adjustment;
    AllocHeader(const Size size, const U64 adjustment)
        : size(size), adjustment(adjustment) {}
  };

  void Erase() const;
  void RemoveNode(Node* node);
  static void InsertNodeAt(Node* pos, Node* newNode);
  static void TryMergeWithNext(Node* node);

  // both are sitting on memHead
  Node* head = nullptr;
  void* memHead = nullptr;
  Action<void*> freeCallback = nullptr;

  // A lesson learned here: if these two variables are not static, it will
  // implicitly involve in the copy constructor's copying process. But as they
  // are const, they can't be copied over. This caused copy constructor to be
  // implicitly declared as deleted
  static const Size nodeSize = sizeof(Node);
  static const Size headerSize = sizeof(AllocHeader);

  friend class MemoryManager;
};

template <typename T, typename... args>
T* FreeListAllocator::New(args... argList) {
  return new (Alloc(sizeof(T), MemUtil::ALIGNMENT)) T(argList...);
}

template <typename T>
T* FreeListAllocator::NewArr(Size length, const U8 alignment) {
  return new (Alloc(sizeof(T) * length, alignment)) T[length];
}

}  // namespace Isetta
