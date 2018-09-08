#include "Memory.h"
#include <iostream>
#include <sstream>
#include "Core/Debug/Logger.h"

namespace Isetta {

std::string HexFromPtr(const PtrInt rawAddress) {
  std::stringstream str;
  str << "Allocated Memory Address: " << rawAddress << " = " << std::hex
      << std::uppercase << rawAddress;
  return str.str();
}

void* AllocateUnaligned(const U32 size) {
  // looks like std::malloc always return 16 byte aligned memory
  return std::malloc(size);
}

void FreeUnaligned(void* mem) { std::free(mem); }

void* AllocateAligned(const U32 size, const U8 alignment) {
  ASSERT(alignment >= 1);
  ASSERT(alignment <= 128);
  ASSERT((alignment & (alignment - 1)) == 0); // power of 2

  const U32 expandedSize = size + alignment;

  const PtrInt rawAddress =
      reinterpret_cast<PtrInt>(AllocateUnaligned(expandedSize));
  // std::string add = HexFromPtr(rawAddress);
  const PtrInt misAlignment = rawAddress & (alignment - 1);
  const PtrDiff adjustment = alignment - misAlignment;
  const PtrInt alignedAddress = rawAddress + adjustment;

  U8* alignedMemory = reinterpret_cast<U8*>(alignedAddress);
  alignedMemory[-1] = adjustment;

  ASSERT(reinterpret_cast<PtrInt>(alignedMemory) % alignment == 0);
  return static_cast<void*>(alignedMemory);
}

void FreeAligned(void* memoryPtr) {
  const U8* alignedMemory = reinterpret_cast<U8*>(memoryPtr);
  const PtrDiff adjustment = static_cast<PtrDiff>(alignedMemory[-1]);
  const PtrInt alignedAddress = reinterpret_cast<PtrInt>(memoryPtr);
  const PtrInt rawAddress = alignedAddress - adjustment;
  void* rawMem = reinterpret_cast<void*>(rawAddress);
  FreeUnaligned(rawMem);
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
  delete (bottom);
}

}  // namespace Isetta
