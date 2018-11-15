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

  // TODO(YIDI): the node can be overriden
  PtrInt rawAddress = reinterpret_cast<PtrInt>(node);
  rawAddress += headerSize;  // leave size for header
  PtrInt misAlignment = rawAddress & (alignment - 1);
  U64 adjustment = alignment - misAlignment;
  adjustment += ((~alignment & adjustment) << 1);
  PtrInt alignedAddress = rawAddress + adjustment;

  Size occupiedSize = headerSize + adjustment + size;
  PtrInt headerAddress = alignedAddress - headerSize;

  // new headers will be reclaimed during "free" process
  // TODO(YIDI): This step maybe buggy, see "Image Creator" page 16, Make sure
  // the node on the left's next is aligned properly
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
  Size totalSize = header->size;
  U64 adjustment = header->adjustment;

  // TODO(YIDI): Take care of double deletion, in that situation, memPtr is
  // the same as head
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

void* FreeListAllocator::Realloc(void* memPtr, Size size, U8 alignment) {
  PtrInt headerAddress = reinterpret_cast<PtrInt>(memPtr) - headerSize;
  auto* header = reinterpret_cast<AllocHeader*>(headerAddress);

  void* newPtr = memPtr;

  PtrInt s = size;

  if (header->size >= size) {
    // If it's smaller than what we had, just shrink our node's size and make a
    // new one

    // TODO(YIDI): the node can be overriden
    PtrInt rawAddress = reinterpret_cast<PtrInt>(memPtr) + s;
    rawAddress += headerSize;  // leave size for header
    PtrInt misAlignment = rawAddress & (alignment - 1);
    U64 adjustment = alignment - misAlignment;
    adjustment += ((~alignment & adjustment) << 1);
    PtrInt alignedAddress = rawAddress + adjustment;

    Size occupiedSize = headerSize + adjustment + (header->size - size);
    PtrInt headerAddress = alignedAddress - headerSize;

    PtrInt nodeAddress =
        reinterpret_cast<PtrInt>(memPtr) + size - header->adjustment;
    auto* newNode =
        new (reinterpret_cast<void*>(nodeAddress)) Node(header->size);

    // TODO(YIDI): Take care of double deletion, in that situation, memPtr is
    // the same as head
    if (head == nullptr) {
      head = newNode;
      return newPtr;
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

  } else {
    // If it's bigger than what we had, grab a whole new chunk of memory and
    // free the old one
    newPtr = Alloc(size, alignment);
    memcpy(newPtr, memPtr, header->size);
    Free(memPtr);
  }

  return newPtr;
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
