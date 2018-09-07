#include "Memory.h"
#include <iostream>
#include <sstream>
#include "Core/Debug/Logger.h"

namespace Isetta {

std::string HexFromPtr(const uintptr_t rawAddress) {
  std::stringstream str;
  str << "Allocated Memory Address: " << rawAddress << " = " << std::hex
      << std::uppercase << rawAddress;
  return str.str();
}

void* AllocateUnaligned(const U32 size) {
  auto* mem = new char[size];
  return static_cast<void*>(mem);
}

void* AllocateAligned(U32 size, U32 alignment) {

}

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

  const auto rawAddress = reinterpret_cast<uintptr_t>(ret);

  Logger::Log(Debug::Channel::Memory, HexFromPtr(rawAddress));

  return static_cast<void*>(ret);
}

void StackAllocator::Erase() {
  Clear();
  delete(bottom);
}

}  // namespace Isetta
