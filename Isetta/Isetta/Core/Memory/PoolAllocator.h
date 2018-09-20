/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {

class PoolAllocator {
 public:
  explicit PoolAllocator(Size chunkSize, Size count);
  ~PoolAllocator() = default;

  void* Get();
  void Free(void*);
  void Erase() const;

 private:
  union Node {
    Node* next;
    explicit Node(Node* next);
  };

  Size capacity;
  Size elementSize;
  Node* head;
  void* memHead;
};

}  // namespace Isetta
