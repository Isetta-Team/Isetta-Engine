#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {
// TODO(YIDI): This class hasn't been tested
class DoubleBufferedAllocator {
 public:
  DoubleBufferedAllocator() = default;
  explicit DoubleBufferedAllocator(Size size);
  ~DoubleBufferedAllocator() = default;
  void* AllocUnAligned(Size size);
  void* Alloc(Size size, U8 alignment = 16);
  void SwapBuffer();
  void ClearCurrentBuffer();
  void Erase();

  template <typename T>
  T* New() {
    void* mem = Alloc(sizeof(T));
    return new (mem) T();
  };

 private:
  StackAllocator stacks[2];
  U8 curStackIndex;
};

}  // namespace Isetta
