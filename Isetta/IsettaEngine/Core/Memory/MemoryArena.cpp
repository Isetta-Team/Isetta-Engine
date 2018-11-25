/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryArena.h"

#include "Core/DataStructures/Array.h"
#include "Core/Debug/Assert.h"
#include "Core/Debug/Logger.h"
#include "Core/Memory/MemUtil.h"
#include "Core/Memory/ObjectHandle.h"
#include "Input/Input.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

// TODO(YIDI): find a way to allow specifying from config
HandleEntry MemoryArena::entryArr[maxHandleCount];

MemoryArena::MemoryArena(const Size size) {
  memHead = std::malloc(size);
  leftAddress = reinterpret_cast<PtrInt>(memHead);
  rightAddress = leftAddress + size;
}

MemoryArena::~MemoryArena() {
  std::free(memHead);
}

// TODO(YIDI): This is only allocating from top
void* MemoryArena::Alloc(const Size size, Size& outSize) {
  PtrInt lastAddress;
  Size lastSize;

  if (addressIndexMap.empty()) {
    lastAddress = leftAddress;
    lastSize = 0;
  } else {
    auto lastPair = --addressIndexMap.end();
    lastAddress = lastPair->first;
    lastSize = entryArr[lastPair->second].size;
  }

  PtrInt rawAddress = lastAddress + lastSize;
  U8 alignment = MemUtil::ALIGNMENT;
  PtrInt misAlignment = rawAddress & (alignment - 1);
  // should not align when misAlignment is 0
  PtrDiff adjustment = (alignment - misAlignment) & (alignment - 1);
  PtrInt alignedAddress = rawAddress + adjustment;

  if (alignedAddress + size > rightAddress) {
    throw std::exception{
        "MemoryArena::Alloc => Not enough memory in the arena!"};
  }

  outSize = size + adjustment;
  return reinterpret_cast<void*>(alignedAddress);
}

void MemoryArena::Defragment() {
  PROFILE
  if (addressIndexMap.empty()) return;

  for (int i = 0; i < 6; ++i) {
    ++curIndex;
    if (curIndex >= addressIndexMap.size()) {
      curIndex = 0;
    }
    MoveLeft(curIndex);
    // LOG_INFO(Debug::Channel::Memory, "Cur size: %I64u", GetUsedSize());
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
  // LOG_INFO(Debug::Channel::Memory, "Trying to align %d", index);

  Array<int> arr;
  arr.Reserve(addressIndexMap.size());
  for (const auto& pair : addressIndexMap) {
    arr.PushBack(pair.second);
  }

  const auto& entry = entryArr[arr[index]];

  PtrInt lastAvailableAddress;
  if (index == 0) {
    lastAvailableAddress = leftAddress;
  } else {
    const auto& lastEntry = entryArr[arr[index - 1]];
    lastAvailableAddress = lastEntry.GetAddress() + lastEntry.size;
  }

  lastAvailableAddress =
      NextMultiplyOfBase(lastAvailableAddress, MemUtil::ALIGNMENT);

  if (lastAvailableAddress < entry.GetAddress()) {
    void* newAdd = reinterpret_cast<void*>(lastAvailableAddress);
    // remove from map and add back
    addressIndexMap.erase(
        addressIndexMap.find(reinterpret_cast<PtrInt>(entry.ptr)));
    addressIndexMap.emplace(reinterpret_cast<PtrInt>(newAdd), arr[index]);

    std::memmove(newAdd, entry.ptr, entry.size);
    // LOG_INFO(Debug::Channel::Memory, "Moving from %p to %p of size %u",
    // entry.ptr, lastAvailableAddress, entry.size);
    entry.ptr = newAdd;
  }
}

void MemoryArena::Print() const {
  LOG_INFO(Debug::Channel::Memory, "[address, index, size]");
  int count = 0;
  for (const auto& pair : addressIndexMap) {
    LOG_INFO(Debug::Channel::Memory, "%d [%p, %d, %u]", count++, pair.first,
             pair.second, entryArr[pair.second].size);
  }
}

PtrInt MemoryArena::GetUsedSize() const {
  PtrInt lastAddress;
  Size lastSize;

  if (addressIndexMap.empty()) {
    lastAddress = leftAddress;
    lastSize = 0;
  } else {
    auto lastPair = --addressIndexMap.end();
    lastAddress = lastPair->first;
    lastSize = entryArr[lastPair->second].size;
  }
  auto lastPair = --addressIndexMap.end();

  return (lastAddress + lastSize) - leftAddress;
}

}  // namespace Isetta
