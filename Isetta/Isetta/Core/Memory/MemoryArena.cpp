/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryArena.h"
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

HandleEntry MemoryArena::entryArr[maxHandleCount];

void* MemoryArena::Alloc(const SizeInt size, SizeInt& outSize,
                         const U8 alignment) {
  const bool isValid = alignment >= 2 && alignment <= 128 &&
                       (alignment & (alignment - 1)) == 0;  // power of 2
  if (!isValid) {
    throw std::invalid_argument(
        "MemoryArena::Alloc => Illegal alignment in allocator");
  }

  PtrInt lastAddress;

  SizeInt lastSize;

  if (addressIndexMap.empty()) {
    lastAddress = leftAddress;
    lastSize = 0;
  } else {
    auto lastPair = --addressIndexMap.end();
    lastAddress = lastPair->first;
    lastSize = entryArr[lastPair->second].size;
  }

  PtrInt rawAddress = lastAddress + lastSize;
  PtrInt misAlignment = rawAddress & (alignment - 1);
  // should not align when misAlignment is 0
  PtrDiff adjustment = (alignment - misAlignment) & (alignment - 1);
  PtrInt alignedAddress = rawAddress + adjustment;

  if (alignedAddress + size > rightAddress) {
    throw std::exception(
        "MemoryArena::Alloc => Not enough memory in the arena left!");
  }

  outSize = size + adjustment;
  return reinterpret_cast<void*>(alignedAddress);
}

void MemoryArena::Defragment() {
  
}

void MemoryArena::Erase() const { std::free(memHead); }

MemoryArena::MemoryArena(const SizeInt size) {
  memHead = std::malloc(size);
  leftAddress = reinterpret_cast<PtrInt>(memHead);
  rightAddress = leftAddress + size;
}

}  // namespace Isetta
