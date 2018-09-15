/*
* Copyright (c) 2018 Isetta
*/
#include "Core/Memory/PoolAllocator.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/MemoryManager.h"

namespace Isetta {

PoolAllocator::PoolAllocator(SizeInt chunkSize, SizeInt count) {
  elementSize = chunkSize;

  if (elementSize > sizeof(Node*)) {
    LOG_ERROR(Debug::Channel::Memory,
              "Using PoolAllocator for chunkSize %d will incur more overhead memory than the memory actually "
                  "needed for the elements", elementSize);
  }

  capacity = count;
  memHead = MemoryAllocator::AllocateDefaultAligned(elementSize * capacity);
  head = new (memHead) Node{nullptr};
  Node* cur = head;

  PtrInt address = reinterpret_cast<PtrInt>(memHead);

  for (SizeInt i = 1; i < count; i++) {
    address += elementSize;
    Node* node = new (reinterpret_cast<void*>(address)) Node{nullptr};
    cur->next = node;
    cur = cur->next;
  }

  isErased = false;
}

PoolAllocator::~PoolAllocator() {
  if (isErased) {
    return;
  }
  isErased = true;
  MemoryAllocator::FreeDefaultAligned(memHead);
}

void* PoolAllocator::Get() {
  if (isErased) {
    throw std::exception("PoolAllocator::Get(): PoolAllocator already erased");
  }

  if (head == nullptr) {
    throw std::out_of_range(
        "PoolAllocator::Get(): Not enough memory in PoolAllocator");
  }

  // TODO(YIDI): Should I initialize memory to zero before return?
  void* ret = head;
  head = head->next;
  return ret;
}

void PoolAllocator::Free(void* mem) {
  if (isErased) {
    throw std::exception("PoolAllocator::Free(): PoolAllocator already erased");
  }
  
  Node* node = new (mem) Node{head};
  head = node;
}

void PoolAllocator::Erase() {
  if (isErased) {
    throw std::exception("PoolAllocator::Get(): PoolAllocator already erased");
  }
  isErased = true;
  MemoryAllocator::FreeDefaultAligned(memHead);
}

PoolAllocator::Node::Node(Node* next) {
  this->next = next;
}


}  // namespace Isetta