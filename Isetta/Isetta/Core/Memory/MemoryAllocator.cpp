/*
* Copyright (c) 2018 Isetta
*/
#include "Core/Memory/MemoryAllocator.h"
#include "Core/Debug/Assert.h"
#include <sstream>

namespace Isetta {

std::string HexFromPtr(const PtrInt rawAddress) {
  std::stringstream str;
  str << "Allocated Memory Address: " << rawAddress << " = " << std::hex
      << std::uppercase << rawAddress;
  return str.str();
}

void* MemoryAllocator::AllocateDefaultAligned(const Size size) {
  // looks like std::malloc always return 16 byte aligned memory
  return std::malloc(size);
}

void MemoryAllocator::FreeDefaultAligned(void* mem) { std::free(mem); }

void* MemoryAllocator::AllocateAligned(const Size size, const U8 alignment) {
  const bool isValid = alignment >= 2 && alignment <= 128 &&
                       (alignment & (alignment - 1)) == 0;  // power of 2
  if (!isValid) {
    throw std::invalid_argument{"Illegal alignment in allocator"};
  }

  const Size expandedSize = size + alignment;

  const PtrInt rawAddress =
      reinterpret_cast<PtrInt>(AllocateDefaultAligned(expandedSize));
  // std::string add = HexFromPtr(rawAddress);
  const PtrInt misAlignment = rawAddress & (alignment - 1);
  const U8 adjustment = alignment - static_cast<U8>(misAlignment);
  const PtrInt alignedAddress = rawAddress + adjustment;

  U8* alignedMemory = reinterpret_cast<U8*>(alignedAddress);
  alignedMemory[-1] = adjustment;

  ASSERT(reinterpret_cast<PtrInt>(alignedMemory) % alignment == 0);
  return static_cast<void*>(alignedMemory);
}

void MemoryAllocator::FreeAligned(void* memoryPtr) {
  const U8* alignedMemory = reinterpret_cast<U8*>(memoryPtr);
  const PtrDiff adjustment = static_cast<PtrDiff>(alignedMemory[-1]);
  const PtrInt alignedAddress = reinterpret_cast<PtrInt>(memoryPtr);
  const PtrInt rawAddress = alignedAddress - adjustment;
  void* rawMem = reinterpret_cast<void*>(rawAddress);
  FreeDefaultAligned(rawMem);
}
}  // namespace Isetta
