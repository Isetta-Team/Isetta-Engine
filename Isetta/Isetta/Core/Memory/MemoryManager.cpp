/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

// TODO(YIDI): Use config for max table size
HandleEntry MemoryManager::handleEntryTable[maxTableSize];
MemoryManager* MemoryManager::instance;

void* MemoryManager::AllocSingleFrameUnAligned(const SizeInt size) {
  return singleFrameAllocator.AllocUnaligned(size);
}

void* MemoryManager::AllocSingleFrame(const SizeInt size,
                                             const U8 alignment) {
  return singleFrameAllocator.Alloc(size, alignment);
}

void* MemoryManager::AllocDoubleBufferedUnAligned(const SizeInt size) {
  return doubleBufferedAllocator.AllocUnAligned(size);
}

void* MemoryManager::AllocDoubleBuffered(const SizeInt size,
                                                const U8 alignment) {
  return doubleBufferedAllocator.Alloc(size, alignment);
}

MemoryManager::MemoryManager() : handlePool(8, maxTableSize){
  // TODO(YIDI): The two allocators are still initialized here by automatically
  // calling their default constructors, find a better way to handle this
  instance = this;
}

void MemoryManager::StartUp() {
  // TODO(YIDI): Get size from the config file
  singleFrameAllocator = StackAllocator(10_MB);
  doubleBufferedAllocator = DoubleBufferedAllocator(10_MB);
}

// Memory Manager's update needs to be called after everything that need memory
// allocation
void MemoryManager::Update() {
  singleFrameAllocator.Clear();
  doubleBufferedAllocator.SwapBuffer();
  doubleBufferedAllocator.ClearCurrentBuffer();
}

void MemoryManager::ShutDown() {
  singleFrameAllocator.Erase();
  doubleBufferedAllocator.Erase();
  handlePool.Erase();
}
}  // namespace Isetta
