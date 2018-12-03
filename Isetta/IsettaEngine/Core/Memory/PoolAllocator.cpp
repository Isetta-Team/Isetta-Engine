/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/PoolAllocator.h"
#include "Util.h"

namespace Isetta {
PoolAllocator::PoolAllocator(const Size chunkSize, const Size count,
                             const Size increment)
    : capacity(count), chunkSize(chunkSize), increment(increment) {
  ASSERT(increment != 0);
  // Using PoolAllocator for chunkSize smaller than pointer will not
  // work because of the memory optimization technique
  ASSERT(chunkSize > sizeof(PoolNode*));

  memHead = MemoryManager::AllocOnFreeList(chunkSize * count);
  InitializeNodes(memHead, count);
}

PoolAllocator::~PoolAllocator() {
  MemoryManager::FreeOnFreeList(memHead);
  for (void* memory : additionalMemory) {
    MemoryManager::FreeOnFreeList(memory);
  }
}

void* PoolAllocator::Get() {
  if (head == nullptr) {
    Expand();
  }

  void* ret = head;
  head = head->next;
  memset(ret, 0x0, sizeof(PoolNode));  // rest is set during free process
  return ret;
}

void PoolAllocator::Free(void* mem) {
  memset(mem, 0x0, chunkSize);
  PoolNode* newHead = new (mem) PoolNode{head};
  head = newHead;
}

void PoolAllocator::InitializeNodes(void* memHead, const Size count) {
  head = new (memHead) PoolNode{nullptr};
  PoolNode* cur = head;
  PtrInt address = reinterpret_cast<PtrInt>(memHead);
  for (Size i = 1; i < count; ++i) {
    address += chunkSize;
    PoolNode* node = new (reinterpret_cast<void*>(address)) PoolNode{nullptr};
    cur->next = node;
    cur = cur->next;
  }
}

void PoolAllocator::Expand() {
  void* newMemHead = MemoryManager::AllocOnFreeList(chunkSize * increment);
  InitializeNodes(newMemHead, increment);
  additionalMemory.PushBack(newMemHead);
  capacity += increment;
  LOG_INFO(Debug::Channel::Memory, "Pool allocator increased by %d", increment);
}
}  // namespace Isetta
