/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemUtil.h"
#include "Util.h"

namespace Isetta {

template <typename T>
class TemplatePoolAllocator {
 public:
  TemplatePoolAllocator() = delete;
  explicit TemplatePoolAllocator(Size count);
  ~TemplatePoolAllocator() { MemUtil::Free(memHead); }

  template <typename... Args>
  T* Get(Args&&...);
  void Free(T*);

 private:
  union PoolNode {
    T t;
    PoolNode* next;
    PoolNode() : next(nullptr) {}
  };

  Size capacity{};
  Size elementSize{};
  PoolNode* head{};
  void* memHead{};
};

template <typename T>
TemplatePoolAllocator<T>::TemplatePoolAllocator(const Size count) {
  if (sizeof(PoolNode*) > sizeof(T)) {
    throw std::exception{Util::StrFormat(
        "TemplatePoolAllocator::TemplatePoolAllocator => Using "
        "TemplatePoolAllocator for type %s will incur more overhead memory "
        "than the memory actually needed for the elements",
        typeid(T).name())};
  }

  capacity = count;
  elementSize = sizeof(T);
  memHead = MemUtil::Alloc(elementSize * capacity);
  PtrInt address = reinterpret_cast<PtrInt>(memHead);
  head = new (memHead) PoolNode();
  PoolNode* cur = head;

  for (Size i = 1; i < count; ++i) {
    address += elementSize;
    auto* node = new (reinterpret_cast<void*>(address)) PoolNode();
    cur->next = node;
    cur = cur->next;
  }
}

template <typename T>
template <typename... Args>
T* TemplatePoolAllocator<T>::Get(Args&&... args) {
  if (head == nullptr) {
    throw std::out_of_range(
        "TemplatePoolAllocator::Get => Not enough memory in "
        "TemplatePoolAllocator");
  }

  PoolNode* next = head->next;
  // head's content will be overriden when constructing T
  // so it's next must be cached before that
  T* t = new (head) T(std::forward<Args>(args)...);
  head = next;
  return t;
}

template <typename T>
void TemplatePoolAllocator<T>::Free(T* t) {
  t->~T();
  auto* node = new (t) PoolNode();
  node->next = head;
  head = node;
}

}  // namespace Isetta
