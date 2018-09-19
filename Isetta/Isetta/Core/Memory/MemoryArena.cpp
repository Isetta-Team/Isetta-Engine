/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryArena.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Utility.h"
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

HandleEntry MemoryArena::entryArr[maxHandleCount];

void* MemoryArena::Alloc(const SizeInt size, SizeInt& outSize) {
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
    throw std::exception{
        "MemoryArena::Alloc => Not enough memory in the arena left!"};
  }

  outSize = size + adjustment;
  return reinterpret_cast<void*>(alignedAddress);
}

void MemoryArena::Defragment() {
  LOG_INFO(Debug::Channel::Memory, "[address, index, size]");
  for (const auto& pair : addressIndexMap) {
    LOG_INFO(Debug::Channel::Memory, "[%lu, %d, %lu]", pair.first, pair.second,
             entryArr[pair.second].size);
  }
}

// TODO(YIDI): move this to math utility
PtrInt NextMultiplyOfBase(const PtrInt number, const U32 base) {
  ASSERT(number != 0);
  ASSERT(base >= 2 && (base & (base - 1)) == 0);

  PtrInt diff = number & (base - 1);
  PtrDiff adjustment = (base - diff) & (base - 1);
  return number + adjustment;
}

void MemoryArena::MoveLeft(const U32 index) {
  ASSERT(index <= addressIndexMap.size() - 1);

  const auto& entry = entryArr[addressIndexMap[index]];
  PtrInt lastAvailableAddress;
  if (index == 0) {
    lastAvailableAddress = leftAddress;
  } else {
    const auto& lastEntry = entryArr[addressIndexMap[index - 1]];
    lastAvailableAddress = lastEntry.GetAddress() + lastEntry.size;
  }

  lastAvailableAddress = NextMultiplyOfBase(lastAvailableAddress, alignment);
  if (lastAvailableAddress < entry.GetAddress()) {
    Move(entry.GetAddress(), lastAvailableAddress, entry.size);
  }
}

void MemoryArena::Erase() const { std::free(memHead); }

void MemoryArena::Move(const PtrInt dest, PtrInt& src, const SizeInt count) {
  std::memmove(reinterpret_cast<void*>(dest), reinterpret_cast<void*>(src),
               count);
}

MemoryArena::MemoryArena(const SizeInt size) {
  memHead = std::malloc(size);
  leftAddress = reinterpret_cast<PtrInt>(memHead);
  rightAddress = leftAddress + size;
}

}  // namespace Isetta
