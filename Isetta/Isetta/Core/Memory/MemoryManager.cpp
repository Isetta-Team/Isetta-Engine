/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryManager.h"
#include "Core/Math/Random.h"
#include "Core/Memory/ObjectHandle.h"
#include "Input/Input.h"
#include "Input/InputEnum.h"
#include <vector>

namespace Isetta {

// TODO(YIDI): Use config for max table size

MemoryManager* MemoryManager::instance;

void* MemoryManager::AllocSingleFrameUnAligned(const Size size) {
  return singleFrameAllocator.AllocUnaligned(size);
}

void* MemoryManager::AllocSingleFrame(const Size size, const U8 alignment) {
  return singleFrameAllocator.Alloc(size, alignment);
}

void* MemoryManager::AllocDoubleBufferedUnAligned(const Size size) {
  return doubleBufferedAllocator.AllocUnAligned(size);
}

void* MemoryManager::AllocDoubleBuffered(const Size size,
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
  dynamicArena = MemoryArena(10_MB);
}

// Memory Manager's update needs to be called after everything that need memory
// allocation
void MemoryManager::Update() {
  singleFrameAllocator.Clear();
  doubleBufferedAllocator.SwapBuffer();
  doubleBufferedAllocator.ClearCurrentBuffer();
  dynamicArena.Defragment();
}

void MemoryManager::ShutDown() {
  singleFrameAllocator.Erase();
  doubleBufferedAllocator.Erase();
  dynamicArena.Erase();
}

void MemoryManager::RegisterCallbacks() {
  Input::RegisterKeyPressCallback(KeyCode::P, [&]() { dynamicArena.Print(); });
}

void MemoryManager::Test() {
  const U32 count = 1024;
  std::vector<ObjectHandle<U64>> arr;

  for (U32 i = 0; i < count; i++) {
    auto ref = NewDynamic<U64>();
    *ref = i;
    arr.push_back(ref);
  }

  auto map = instance->dynamicArena.addressIndexMap;

  for (U32 i = 0; i < count / 2; i++) {
    int index = Math::Random::GetRandomGenerator(0, arr.size() - 1).GetValue();
    DeleteDynamic(arr[index]);
    arr.erase(arr.begin() + index);
  }
}
}  // namespace Isetta
