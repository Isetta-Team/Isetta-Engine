/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "PoolAllocator.h"

namespace Isetta {

template <typename T>
class TemplatePoolAllocator {
 public:
  TemplatePoolAllocator() = delete;
  explicit TemplatePoolAllocator(const Size count, const Size increment)
      : pool(sizeof(T), count, increment) {}
  ~TemplatePoolAllocator() = default;

  template <typename... Args>
  T* Get(Args&&... args) {
    return new (pool.Get()) T(std::forward<Args>(args)...);
  }
  void Free(T* t) {
    t->~T();
    pool.Free(t);
  }

 private:
  PoolAllocator pool;
};
}  // namespace Isetta
