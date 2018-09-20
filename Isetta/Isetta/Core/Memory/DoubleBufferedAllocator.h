/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {
// TODO(YIDI): This class hasn't been tested
class DoubleBufferedAllocator {
 public:
  ~DoubleBufferedAllocator() = default;

 private:
  DoubleBufferedAllocator() = default;
  explicit DoubleBufferedAllocator(SizeInt size);
  void* AllocUnAligned(SizeInt size);
  void* Alloc(SizeInt size, U8 alignment = 16);
  void SwapBuffer();
  void ClearCurrentBuffer();
  void Erase();

  template <typename T>
  T* New() {
    void* mem = Alloc(sizeof(T));
    return new (mem) T();
  }

  StackAllocator stacks[2];
  U8 curStackIndex;
  friend class MemoryManager;
};

}  // namespace Isetta
