/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {

class PoolAllocator {
 public:
  explicit PoolAllocator(SizeInt chunkSize, SizeInt count);
  ~PoolAllocator() = default;

  void* Get();
  void Free(void*);
  void Erase() const;

 private:
  union Node {
    Node* next;
    Node(Node* next);
  };

  SizeInt capacity;
  SizeInt elementSize;
  Node* head;
  void* memHead;
};

}  // namespace Isetta