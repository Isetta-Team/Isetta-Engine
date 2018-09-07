#include "Memory.h"
#include "Core/Debug/Logger.h"

namespace Isetta {


StackAllocator::StackAllocator(const U32 stackSize)
    : top(0), capacity(stackSize) {
  bottom = new char[stackSize];
}

void* StackAllocator::Alloc(const U32 sizeInBytes) {
  const Marker newTop = top + sizeInBytes;
  if (newTop > capacity) {
    Logger::LogError(Debug::Channel::Memory,
                     "Not enough memory in stack allocator");
    
  }

  top = newTop;
  char* ret = bottom + sizeInBytes;
  return static_cast<void*>(ret);
}

void StackAllocator::Erase() {
  Clear();
  free(bottom);
}


}  // namespace Isetta
