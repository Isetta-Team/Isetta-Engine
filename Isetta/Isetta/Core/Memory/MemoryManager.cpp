/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

// TODO(YIDI): Use config for max table size
HandleEntry MemoryManager::handleTable[maxTableSize];

MemoryManager::MemoryManager() {
  // TODO(YIDI): The two allocators are still initialized here by automatically calling their
  // default constructors, find a better way to handle this
}

void MemoryManager::StartUp() {
  // TODO(YIDI): Get size from the config file
  singleFrameAllocator = StackAllocator(10_MB);
  doubleBufferedAllocator = DoubleBufferedAllocator(10_MB);
}

// Memory Manager's update needs to be called after everything that need memory allocation
void MemoryManager::Update() {
  singleFrameAllocator.Clear();
  doubleBufferedAllocator.SwapBuffer();
  doubleBufferedAllocator.ClearCurrentBuffer();
}

void MemoryManager::ShutDown() {
  singleFrameAllocator.Erase();
  doubleBufferedAllocator.Erase();
}
}  // namespace Isetta
