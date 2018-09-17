/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryAllocator.h"

namespace Isetta {

template <typename T>
class TemplatePoolAllocator {
 public:
  explicit TemplatePoolAllocator(SizeInt count);
  ~TemplatePoolAllocator();
  T* Get();
  void Free(T*);
  void Erase();

 private:
  union Node {
    T t;
    Node* next;
    Node();
  };

  SizeInt capacity;
  SizeInt elementSize;
  Node* head;
  void* memHead;
  bool isErased;
};

template <typename T>
TemplatePoolAllocator<T>::TemplatePoolAllocator(const SizeInt count) {
  if (sizeof(Node*) > sizeof(T)) {
    LOG_ERROR(Debug::Channel::Memory,
              "Using TemplatePoolAllocator for type %s will incur more overhead memory than the memory actually "
                  "needed for the elements", typeid(T).name());
    return;
  }

  isErased = false;
  capacity = count;
  elementSize = sizeof(T);
  memHead = MemoryAllocator::AllocateDefaultAligned(elementSize * capacity);
  PtrInt address = reinterpret_cast<PtrInt>(memHead);
  head = new (memHead) Node();
  Node* cur = head;

  for (SizeInt i = 1; i < count; i++) {
    address += elementSize;
    Node* node = new (reinterpret_cast<void*>(address)) Node();
    cur->next = node;
    cur = cur->next;
  }
}

template <typename T>
TemplatePoolAllocator<T>::~TemplatePoolAllocator() {
  Erase();
}

template <typename T>
T* TemplatePoolAllocator<T>::Get() {
  if (isErased) {
    throw std::exception("TemplatePoolAllocator::Get(): TemplatePoolAllocator already erased");
  }

  if (head == nullptr) {
    throw std::out_of_range(
        "TemplatePoolAllocator::Get(): Not enough memory in TemplatePoolAllocator");
  }

  Node* next = head->next;
  // head's content will be overriden when constructing T
  // so it's next must be cached before that
  T* t = new (head) T(); 
  head = next;
  return t;
}

template <typename T>
void TemplatePoolAllocator<T>::Free(T* t) {
  if (isErased) {
    LOG_ERROR(Debug::Channel::Memory, "TemplatePoolAllocator::Free(): TemplatePoolAllocator already erased");
  }
  t->~T();
  Node* node = new (t) Node();
  node->next = head;
  head = node;
}

template <typename T>
void TemplatePoolAllocator<T>::Erase() {
  if (isErased) {
    LOG_ERROR(Debug::Channel::Memory, "TemplatePoolAllocator::Erase(): TemplatePoolAllocator already erased");
    return;
  }
  isErased = true;
  MemoryAllocator::FreeDefaultAligned(memHead);
}

template <typename T>
TemplatePoolAllocator<T>::Node::Node() {
  next = nullptr;
}
}  // namespace Isetta