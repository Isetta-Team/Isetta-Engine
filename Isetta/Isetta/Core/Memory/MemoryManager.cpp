/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

// TODO(YIDI): Use config for max table size
HandleEntry MemoryManager::handleTable[maxTableSize];

MemoryManager::MemoryManager(): singleFrameAllocator(1024), doubleBufferedAllocator(1024) {

}

void MemoryManager::StartUp() {
}

void MemoryManager::Update() {
}

void MemoryManager::ShutDown() {
}
}  // namespace Isetta
