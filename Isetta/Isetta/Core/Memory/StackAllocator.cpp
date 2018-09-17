/*
* Copyright (c) 2018 Isetta
*/

#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryAllocator.h"
#include "Core/Memory/StackAllocator.h"
#include <stdexcept>
#include "Core/Debug/Logger.h"

namespace Isetta {
StackAllocator::StackAllocator(const SizeInt stackSize)
  : top(0), length(stackSize), isErased(false) {
  bottom = MemoryAllocator::AllocateDefaultAligned(stackSize);
  bottomAddress = reinterpret_cast<PtrInt>(bottom);
}

StackAllocator::~StackAllocator() {
  Erase();
}

void* StackAllocator::AllocAligned(const SizeInt size, const U8 alignment) {
  if (isErased) {
    LOG_ERROR(Debug::Channel::Memory, "StackAllocator::AllocUnaligned() : Stack Allocator is already erased");
    return nullptr;
  }

  const bool isValid = alignment >= 2 && alignment <= 128 &&
                       (alignment & (alignment - 1)) == 0;  // power of 2
  if (!isValid) {
    throw std::invalid_argument("Illegal alignment in allocator");
  }

  PtrInt rawAddress = bottomAddress + top;
  PtrInt misAlignment = rawAddress & (alignment - 1);
  PtrDiff adjustment = alignment - misAlignment;
  // for the special case when misAlignment = 0
  // make sure we don't shift the address by its alignment
  adjustment = adjustment & (alignment - 1); 
  PtrInt alignedAddress = rawAddress + adjustment;
  Marker newTop = top + size + adjustment;

  if (newTop > length) {
    // TODO(YIDI): should I throw an exception here?
    throw std::overflow_error("Not enough memory in stack allocator");
  }

  top = newTop;

  return reinterpret_cast<void*>(alignedAddress);
}

void* StackAllocator::AllocUnaligned(SizeInt size) {
  if (isErased) {
    LOG_ERROR(Debug::Channel::Memory, "StackAllocator::AllocUnaligned() : Stack Allocator is already erased");
    return nullptr;
  }

  Marker newTop = top + size;

  if (newTop > length) {
    // TODO(YIDI): should I throw an exception here?
    throw std::overflow_error("Not enough memory in stack allocator");
  }

  void* mem = reinterpret_cast<void*>(bottomAddress + top);
  top = newTop;
  return mem;
}

void StackAllocator::Erase() {
  if (isErased) {
    LOG_ERROR(Debug::Channel::Memory, "StackAllocator::Erase() : Stack Allocator is already erased");
    return;
  }
  Clear();
  std::free(bottom);
  isErased = true;
}
}
