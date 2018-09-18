/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

// TODO(YIDI): Use config for max table size
HandleEntry MemoryManager::entryArr[maxHandleCount];
MemoryManager* MemoryManager::instance;

void* MemoryManager::AllocSingleFrameUnAligned(const SizeInt size) {
  return singleFrameAllocator.AllocUnaligned(size);
}

void* MemoryManager::AllocSingleFrame(const SizeInt size, const U8 alignment) {
  return singleFrameAllocator.Alloc(size, alignment);
}

void* MemoryManager::AllocDoubleBufferedUnAligned(const SizeInt size) {
  return doubleBufferedAllocator.AllocUnAligned(size);
}

void* MemoryManager::AllocDoubleBuffered(const SizeInt size,
                                         const U8 alignment) {
  return doubleBufferedAllocator.Alloc(size, alignment);
}

MemoryManager::MemoryManager() {
  // TODO(YIDI): The two allocators are still initialized here by automatically
  // calling their default constructors, find a better way to handle this
  instance = this;
}

void MemoryManager::StartUp() {
  // TODO(YIDI): Get size from the config file
  singleFrameAllocator = StackAllocator(10_MB);
  doubleBufferedAllocator = DoubleBufferedAllocator(10_MB);
  dynamicArena = StackAllocator(20_MB);
}

// Memory Manager's update needs to be called after everything that need memory
// allocation
void MemoryManager::Update() {
  singleFrameAllocator.Clear();
  doubleBufferedAllocator.SwapBuffer();
  doubleBufferedAllocator.ClearCurrentBuffer();
  Defragment();
}

void MemoryManager::ShutDown() {
  singleFrameAllocator.Erase();
  doubleBufferedAllocator.Erase();
  dynamicArena.Erase();
}

// TODO(YIDI): Implemented this
void MemoryManager::Defragment() {
  LOG_INFO(Debug::Channel::Memory, "[Address, index, size]");
  for (auto& pair : addressIndexMap) {
    PtrInt address = pair.first;
    int index = pair.second;
    LOG_INFO(Debug::Channel::Memory, "[%lu, %d, %lu]", address, index, entryArr[index].size);
  }
}

// TODO(YIDI): Implement this
void* MemoryManager::AllocDynamic(const SizeInt size, const U8 alignment) {
  return instance->dynamicArena.Alloc(size, alignment);
}

// TODO(YIDI): Implemented this
void MemoryManager::FreeDynamic(void* ptrToFree) {}
}  // namespace Isetta
