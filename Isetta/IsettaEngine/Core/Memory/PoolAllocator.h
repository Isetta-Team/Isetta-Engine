/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/DataStructures/Array.h"

namespace Isetta {
class PoolAllocator {
 public:
  PoolAllocator() = delete;
  explicit PoolAllocator(Size chunkSize, Size count, Size increment);
  ~PoolAllocator();
  void* Get();
  void Free(void*);

 private:
  union PoolNode {
    PoolNode* next;
    explicit PoolNode(PoolNode* next) { this->next = next; }
  };

  void InitializeNodes(void* memHead, Size count);
  void Expand();

  Size capacity{};
  Size chunkSize{};
  Size increment{};
  PoolNode* head{};
  void* memHead{};
  Array<void*> additionalMemory;
};

}  // namespace Isetta
