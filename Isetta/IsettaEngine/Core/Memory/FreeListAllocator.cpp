/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/FreeListAllocator.h"
#include "Core/Memory/MemUtil.h"
#include "Util.h"

namespace Isetta {

FreeListAllocator::FreeListAllocator(const Size size) {
  memHead = std::malloc(size);
  head = new (memHead) Node(size);
}

FreeListAllocator::FreeListAllocator(void* headPtr, const Size size,
                                     Action<void*> freeCallback)
    : memHead(headPtr), freeCallback(std::move(freeCallback)) {
  head = new (memHead) Node(size);
}

void* FreeListAllocator::Alloc(const Size size, const U8 alignment) {
  if (head == nullptr) {
    throw std::exception{
        Util::StrFormat("FreeListAllocator::Alloc => No space left")};
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
    throw std::exception{
        Util::StrFormat("FreeListAllocator::Alloc => No node is big enough for "
                        "the new request")};
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

  return reinterpret_cast<void*>(alignedAddress);
}

void FreeListAllocator::Free(void* memPtr) {
  PtrInt allocHeaderAdd = reinterpret_cast<PtrInt>(memPtr) - headerSize;
  auto* allocHeader = reinterpret_cast<AllocHeader*>(allocHeaderAdd);
  PtrInt nodeAddress = allocHeaderAdd - allocHeader->adjustment;
  auto* newNode = new (reinterpret_cast<void*>(nodeAddress)) Node(allocHeader->size);

  if (head == nullptr) {
    head = newNode;
    return;
  }

  // find last and next node, try to merge them
  if (nodeAddress < reinterpret_cast<PtrInt>(head)) {
    newNode->next = head;
    head = newNode;
    TryMergeWithNext(head);
  } else {
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

void* FreeListAllocator::Realloc(void* memPtr, const Size newSize, const U8 alignment) {
  PtrInt allocHeaderAdd = reinterpret_cast<PtrInt>(memPtr) - headerSize;
  auto* allocHeader = reinterpret_cast<AllocHeader*>(allocHeaderAdd);

  void* dest = Alloc(newSize, alignment);
  memcpy(dest, memPtr, allocHeader->size);
  Free(memPtr);
  return dest;
}

void FreeListAllocator::Erase() const {
  if (memHead == nullptr) {
    return;
  }

  if (freeCallback == nullptr) {
    std::free(memHead);
  } else {
    freeCallback(memHead);
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
    // the original node->next is effectively deleted cause no one has reference to it
  }
}

}  // namespace Isetta
