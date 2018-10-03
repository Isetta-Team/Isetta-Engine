/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemUtil.h"
#include "Core/Debug/Assert.h"

namespace Isetta {

void* MemUtil::Alloc(const Size size, const U8 alignment) {
  CheckAlignment(alignment);

  const Size expandedSize = size + alignment;

  const PtrInt rawAddress = reinterpret_cast<PtrInt>(std::malloc(expandedSize));
  // std::string add = HexFromPtr(rawAddress);
  const PtrInt misAlignment = rawAddress & (alignment - 1);
  const U8 adjustment = alignment - static_cast<U8>(misAlignment);
  const PtrInt alignedAddress = rawAddress + adjustment;

  U8* alignedMemory = reinterpret_cast<U8*>(alignedAddress);
  alignedMemory[-1] = adjustment;

  ASSERT(reinterpret_cast<PtrInt>(alignedMemory) % alignment == 0);
  return static_cast<void*>(alignedMemory);
}

void MemUtil::Free(void* memoryPtr) {
  const U8* alignedMemory = reinterpret_cast<U8*>(memoryPtr);
  const PtrDiff adjustment = static_cast<PtrDiff>(alignedMemory[-1]);
  const PtrInt alignedAddress = reinterpret_cast<PtrInt>(memoryPtr);
  const PtrInt rawAddress = alignedAddress - adjustment;
  void* rawMem = reinterpret_cast<void*>(rawAddress);
  std::free(rawMem);
}

void MemUtil::CheckAlignment(const U8 alignment) {
  const bool isValid = alignment >= 8 && alignment <= 128 &&
                       (alignment & (alignment - 1)) == 0;  // power of 2
  if (!isValid) {
    throw std::invalid_argument{
        "MemoryAllocator::CheckAlignment => Illegal alignment, must satisfy: "
        "(alignment "
        ">= 8 && alignment <= 128 &&"
        "(alignment & (alignment - 1)) == 0)"};
  }
}
}  // namespace Isetta
