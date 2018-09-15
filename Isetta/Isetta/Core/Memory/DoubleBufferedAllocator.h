#pragma once
#include "Core/IsettaAlias.h"
#include "StackAllocator.h"

namespace Isetta {

class DoubleBufferedAllocator {
public:
  explicit DoubleBufferedAllocator(SizeInt size);
  ~DoubleBufferedAllocator();
  void* Alloc(SizeInt size, U8 alignment = 16);
  void SwapBuffer();
  void ClearCurrentBuffer();
  
private:
  StackAllocator stacks[2];
  U8 curStackIndex;
};

}  // namespace Isetta
