/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/PoolAllocator.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/MemoryAllocator.h"
#include "Utilities.h"

namespace Isetta {

PoolAllocator::PoolAllocator(const Size chunkSize, const Size count) {
  elementSize = chunkSize;

  if (elementSize > sizeof(Node*)) {
    throw std::exception{Utilities::Msg(
        "PoolAllocator::PoolAllocator => Using PoolAllocator for chunkSize %d "
        "will incur more overhead memory than the memory actually "
        "needed for the elements",
        elementSize)};
  }

  capacity = count;
  memHead = MemoryAllocator::AllocateDefaultAligned(elementSize * capacity);
  head = new (memHead) Node{nullptr};
  Node* cur = head;

  PtrInt address = reinterpret_cast<PtrInt>(memHead);

  for (Size i = 1; i < count; i++) {
    address += elementSize;
    Node* node = new (reinterpret_cast<void*>(address)) Node{nullptr};
    cur->next = node;
    cur = cur->next;
  }
}

void* PoolAllocator::Get() {
  if (head == nullptr) {
    throw std::out_of_range{"PoolAllocator::Get => Not enough memory"};
  }

  // TODO(YIDI): Should I initialize memory to zero before return?
  void* ret = head;
  head = head->next;
  return ret;
}

void PoolAllocator::Free(void* mem) {
  Node* node = new (mem) Node{head};
  head = node;
}

void PoolAllocator::Erase() const {
  MemoryAllocator::FreeDefaultAligned(memHead);
}

PoolAllocator::Node::Node(Node* next) { this->next = next; }
}  // namespace Isetta
