#pragma once
#include "Core/IsettaAlias.h"
#include "StackAllocator.h"
#include "DoubleBufferedAllocator.h"

namespace Isetta {

class MemoryManager {
public:
  template<typename T>
  T* SingleFrameNew();

private:
  explicit MemoryManager() = default;
  ~MemoryManager() = default;

  void StartUp();
  void Update();
  void ShutDown();

  StackAllocator singleFrameAllocator;
  DoubleBufferedAllocator doubleBufferedAllocator;
};

}  // namespace Isetta
