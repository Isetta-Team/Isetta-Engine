/*
 * Copyright (c) 2018 Isetta
 */

#include "Core/Memory/StackAllocator.h"
#include <stdexcept>
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemUtil.h"

namespace Isetta {
StackAllocator::StackAllocator(const Size stackSize)
    : top(0), totalSize(stackSize) {
  bottom = std::malloc(stackSize);
  bottomAddress = reinterpret_cast<PtrInt>(bottom);
}

StackAllocator::~StackAllocator() {
  std::free(bottom);
}

void* StackAllocator::Alloc(const Size size, const U8 alignment) {
  MemUtil::CheckAlignment(alignment);

  PtrInt rawAddress = bottomAddress + top;
  PtrInt misAlignment = rawAddress & (alignment - 1);
  PtrDiff adjustment = alignment - misAlignment;
  // for the special case when misAlignment = 0
  // make sure we don't shift the address by its alignment
  adjustment = adjustment & (alignment - 1);
  PtrInt alignedAddress = rawAddress + adjustment;
  Marker newTop = top + size + adjustment;

  if (newTop > totalSize) {
    throw std::overflow_error{"StackAllocator::Alloc => Not enough memory"};
  }

  top = newTop;

  return reinterpret_cast<void*>(alignedAddress);
}

}  // namespace Isetta
