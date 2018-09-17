#pragma once
#include "Core/IsettaAlias.h"
#include "StackAllocator.h"

namespace Isetta {

class DoubleBufferedAllocator {
public:
  DoubleBufferedAllocator() = default;
  explicit DoubleBufferedAllocator(SizeInt size);
  ~DoubleBufferedAllocator();
  void* Alloc(SizeInt size, U8 alignment = 16);
  void SwapBuffer();
  void ClearCurrentBuffer();
  void Erase();
  
private:
  StackAllocator stacks[2];
  U8 curStackIndex;
};

}  // namespace Isetta
