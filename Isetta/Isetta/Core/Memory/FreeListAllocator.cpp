/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/FreeListAllocator.h"
#include "Core/Memory/MemoryAllocator.h"
#include "Util.h"

namespace Isetta {

FreeListAllocator::FreeListAllocator(const Size size) {
  memHead = std::malloc(size);
  head = new (memHead) Node(size);
}

FreeListAllocator::~FreeListAllocator() {
  std::free(memHead);
}

// FreeListAllocator::FreeListAllocator(void* headPtr, const Size size)
// : memHead(headPtr) {
// head = new (memHead) Node(size);
// }

void* FreeListAllocator::Alloc(const Size size, const U8 alignment) {
  if (head == nullptr) {
    throw std::exception{
        Util::StrFormat("FreeListAllocator::Alloc => No space left")};
  }

  MemoryAllocator::CheckAlignment(alignment);

  Size need = headerSize + alignment + size;
  Node* cur = head;
  Node* node = nullptr;

  while (cur != nullptr) {
    if (cur->size >= need) {
      node = cur;
      break;
    }
    cur = cur->next;
  }

  if (node == nullptr) {
    throw std::exception{
        Util::StrFormat("FreeListAllocator::Alloc => No node is big enough for "
                        "the new request")};
  }

  PtrInt rawAddress = reinterpret_cast<PtrInt>(node);
  rawAddress += headerSize;  // leave size for header
  PtrInt misAlignment = rawAddress & (alignment - 1);
  U64 adjustment = alignment - misAlignment;
  PtrInt alignedAddress = rawAddress + adjustment;

  Size occupiedSize = headerSize + adjustment + size;
  PtrInt headerAddress = alignedAddress - headerSize;

  // new headers will be reclaimed during "free" process
  if (node->size >= occupiedSize + nodeSize) {
    // enough space to put a node here
    auto* header = new (reinterpret_cast<void*>(headerAddress))
        AllocHeader(occupiedSize, adjustment);

    InsertNodeAt(node, new (reinterpret_cast<void*>(alignedAddress + size))
                           Node(node->size - occupiedSize));
  } else {
    // not enough space left for node
    auto* header = new (reinterpret_cast<void*>(headerAddress))
        AllocHeader(node->size, adjustment);
  }

  RemoveNode(node);

  return reinterpret_cast<void*>(alignedAddress);
}

void FreeListAllocator::Free(void* memPtr) {
  PtrInt headerAddress = reinterpret_cast<PtrInt>(memPtr) - headerSize;
  auto* header = reinterpret_cast<AllocHeader*>(headerAddress);
  PtrInt nodeAddress = headerAddress - header->adjustment;
  auto* newNode = new (reinterpret_cast<void*>(nodeAddress)) Node(header->size);

  if (head == nullptr) {
    head = newNode;
    return;
  }

  // find last and next node, try to merge them
  if (nodeAddress < reinterpret_cast<PtrInt>(head)) {
    newNode->next = head;
    head = newNode;
    // TODO(YIDI): Go through this with a test case (make sure there won't be
    // gaps anywhere in this list)
    TryMergeWithNext(head);
  } else {
    Node* last = nullptr;
    Node* cur = head;
    while (reinterpret_cast<PtrInt>(cur) < nodeAddress) {
      last = cur;
      cur = cur->next;
    }
    last->next = newNode;
    newNode->next = cur;
    TryMergeWithNext(newNode);
    TryMergeWithNext(last);
  }
}

void FreeListAllocator::RemoveNode(Node* node) {
  Node* last = nullptr;

  if (node == head) {
    head = node->next;
    return;
  }

  Node* cur = head;
  while (cur != nullptr) {
    if (cur->next == node) {
      last = cur;
      break;
    }
    cur = cur->next;
  }

  if (last == nullptr) {
    throw std::exception{
        Util::StrFormat("FreeListAllocator::RemoveNode => Node not found!")};
  }

  last->next = node->next;
}

void FreeListAllocator::InsertNodeAt(Node* pos, Node* newNode) {
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
  }
}

}  // namespace Isetta
