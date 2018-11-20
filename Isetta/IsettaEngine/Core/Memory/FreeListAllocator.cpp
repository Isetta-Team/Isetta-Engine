/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/FreeListAllocator.h"
#include <iostream>
#include "Core/Config/Config.h"
#include "Core/Memory/MemUtil.h"
#include "Scene/Entity.h"

namespace Isetta {

FreeListAllocator::FreeListAllocator(const Size size) {
  memHead = std::malloc(size);
  head = new (memHead) Node(size);
#if _DEBUG
  totalSize += size;
  LOG_INFO(Debug::Channel::Memory, "A new free list of size %d is created",
           size);
#endif
}

void* FreeListAllocator::Alloc(const Size size, const U8 alignment) {
  if (head == nullptr) {
    Expand();
    return Alloc(size, alignment);
  }

  MemUtil::CheckAlignment(alignment);

  Size need = headerSize + alignment + size;
  Node* last = nullptr;
  Node* cur = head;
  Node* node = nullptr;

  while (cur != nullptr) {
    if (cur->size >= need) {
      node = cur;
      break;
    }
    last = cur;
    cur = cur->next;
  }

  if (node == nullptr) {
    Expand();
    return Alloc(size, alignment);
  }

  PtrInt rawAddress = reinterpret_cast<PtrInt>(node);
  rawAddress += headerSize;  // leave size for header
  PtrInt misAlignment = rawAddress & (alignment - 1);
  U64 adjustment = alignment - misAlignment;
  PtrInt alignedAddress = rawAddress + adjustment;
  PtrInt headerAddress = alignedAddress - headerSize;

  Size occupiedSize = headerSize + adjustment + size;
  Size allocSize;

  if (node->size >= occupiedSize + nodeSize) {
    // enough space to put a node here
    Node* newNode = new (reinterpret_cast<void*>(alignedAddress + size))
        Node(node->size - occupiedSize);

    InsertNodeAt(node, newNode);
    RemoveNode(last, node);
    allocSize = occupiedSize;
  } else {
    // not enough space left for node
    RemoveNode(last, node);
    allocSize = node->size;
  }

  // new headers will be reclaimed during "free" process
  new (reinterpret_cast<void*>(headerAddress))
      AllocHeader(allocSize, adjustment);
#if _DEBUG
  sizeUsed += allocSize;
  std::cout << sizeUsed << " ++ " << allocSize << std::endl;
#endif

  void* ret = reinterpret_cast<void*>(alignedAddress);
  memset(ret, 0, size);
  return ret;
}

void FreeListAllocator::Free(void* memPtr) {
  PtrInt allocHeaderAdd = reinterpret_cast<PtrInt>(memPtr) - headerSize;
  auto* allocHeader = reinterpret_cast<AllocHeader*>(allocHeaderAdd);
#if _DEBUG
  sizeUsed -= allocHeader->size;
  std::cout << sizeUsed << " -- " << allocHeader->size << std::endl;
#endif
  PtrInt nodeAddress = allocHeaderAdd - allocHeader->adjustment;
  auto* newNode =
      new (reinterpret_cast<void*>(nodeAddress)) Node(allocHeader->size);
  memset(newNode + 1, 0xD, newNode->size - nodeSize);

  InsertNode(newNode);
}

void* FreeListAllocator::Realloc(void* memPtr, const Size newSize,
                                 const U8 alignment) {
  PtrInt allocHeaderAdd = reinterpret_cast<PtrInt>(memPtr) - headerSize;
  auto* allocHeader = reinterpret_cast<AllocHeader*>(allocHeaderAdd);

  void* dest = Alloc(newSize, alignment);
  memcpy(dest, memPtr, Math::Util::Min(allocHeader->size, newSize));
  Free(memPtr);
  return dest;
}

void FreeListAllocator::Expand() {
  Size increment = CONFIG_VAL(memoryConfig.freeListIncrement);
  void* newMem = std::malloc(increment);
  Node* newNode = new (newMem) Node{increment};
  InsertNode(newNode);
  additionalMemory.push_back(newNode);

#if _DEBUG
  totalSize += increment;
#endif
}

void FreeListAllocator::Erase() const {
  if (memHead == nullptr) {
    return;
  }

#if _DEBUG
  LOG_WARNING(Debug::Channel::Memory, "Memory leak of %I64u detect on freelist",
              sizeUsed);
#endif

  std::free(memHead);
  for (void* const memory : additionalMemory) {
    std::free(memory);
  }
}

void FreeListAllocator::RemoveNode(Node* last, Node* nodeToRemove) {
  if (nodeToRemove == head) {
    head = nodeToRemove->next;
    return;
  }

  ASSERT(last != nullptr);
  last->next = nodeToRemove->next;
}

void FreeListAllocator::InsertNode(Node* newNode) {
  if (head == nullptr) {
    head = newNode;
    return;
  }

  PtrInt nodeAddress = reinterpret_cast<PtrInt>(newNode);

  // find last and next node, try to merge them
  if (nodeAddress < reinterpret_cast<PtrInt>(head)) {
    // New node is on the left of head node
    newNode->next = head;
    head = newNode;
    TryMergeWithNext(head);
  } else {
    // New node is on the right of head node
    Node* last = nullptr;
    Node* cur = head;
    while (reinterpret_cast<PtrInt>(cur) < nodeAddress && cur != nullptr) {
      last = cur;
      cur = cur->next;
    }
    last->next = newNode;
    newNode->next = cur;
    TryMergeWithNext(newNode);
    TryMergeWithNext(last);
  }
  // TODO(Yidi)
  // memset((void*)(headerAddress + headerSize), NULL,
  //       totalSize - headerSize - adjustment);
}

void FreeListAllocator::InsertNodeAt(Node* pos, Node* newNode) {
  ASSERT(pos != nullptr && newNode != nullptr);
  newNode->next = pos->next;
  pos->next = newNode;
}

void FreeListAllocator::TryMergeWithNext(Node* node) {
  if (node->next == nullptr) {
    return;
  }

  // i.e. startAddress of next node
  PtrInt endAddress = reinterpret_cast<PtrInt>(node) + node->size;
  if (endAddress == reinterpret_cast<PtrInt>(node->next)) {
    // if the adjacent next address is a node
    node->size += node->next->size;
    node->next = node->next->next;
    memset(node + 1, 0xD, node->size - nodeSize);
    // the original node->next is effectively deleted cause no one has reference
    // to it
  }
}

#if _DEBUG
void FreeListAllocator::Print() const {
  const int interval = 180;
  static int i = interval;
  ++i;
  if (i > interval) {
    LOG_INFO(Debug::Channel::Memory, "Freelist usage: %I64u / %I64u = %.3f%%",
             sizeUsed, totalSize,
             static_cast<float>(sizeUsed) / totalSize * 100);
    i = 0;
  }
}
#endif

}  // namespace Isetta
