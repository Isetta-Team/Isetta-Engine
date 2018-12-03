/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/MemoryManager.h"
#include "Core/Config/Config.h"
#include "Core/DataStructures/Array.h"
#include "Core/Math/Random.h"
#include "Core/Memory/ObjectHandle.h"
#include "Util.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {

MemoryManager* MemoryManager::instance;

void* MemoryManager::AllocOnSingleFrame(const Size size, const U8 alignment) {
  return GetInstance()->singleFrameAllocator.Alloc(size, alignment);
}

void* MemoryManager::AllocOnDoubleBuffered(const Size size,
                                           const U8 alignment) {
  return GetInstance()->doubleBufferedAllocator.Alloc(size, alignment);
}

void* MemoryManager::AllocOnStack(const Size size, const U8 alignment) {
  return GetInstance()->lsrAndLevelAllocator.Alloc(size, alignment);
}

void* MemoryManager::AllocOnFreeList(const Size size, const U8 alignment) {
  return GetInstance()->freeListAllocator.Alloc(size, alignment);
}

void* MemoryManager::ReallocOnFreeList(void* memPtr, const Size newSize,
                                       const U8 alignment) {
  return GetInstance()->freeListAllocator.Realloc(memPtr, newSize, alignment);
}

void MemoryManager::FreeOnFreeList(void* memPtr) {
  GetInstance()->freeListAllocator.Free(memPtr);
}

MemoryManager::MemoryManager()
    : lsrAndLevelAllocator(CONFIG_VAL(memoryConfig.lsrAndLevelAllocatorSize)),
      singleFrameAllocator(CONFIG_VAL(memoryConfig.singleFrameAllocatorSize)),
      doubleBufferedAllocator(
          CONFIG_VAL(memoryConfig.doubleBufferedAllocatorSize)),
      dynamicArena(CONFIG_VAL(memoryConfig.dynamicArenaSize)),
      freeListAllocator(CONFIG_VAL(memoryConfig.freeListAllocatorSize)) {
  instance = this;
}

// Memory Manager's update needs to be called after everything that need memory
// allocation
void MemoryManager::Update() {
  BROFILER_CATEGORY("Memory Update", Profiler::Color::Teal);

  singleFrameAllocator.Clear();
  doubleBufferedAllocator.SwapBuffer();
  doubleBufferedAllocator.ClearCurrentBuffer();
  dynamicArena.Defragment();
#if _DEBUG
  // freeListAllocator.Print();
#endif
}

void MemoryManager::FinishEngineStartupListener() {
  lvlMemStartMarker = lsrAndLevelAllocator.GetMarker();
}

void MemoryManager::ClearLevelMemory() {
  lsrAndLevelAllocator.FreeToMarker(lvlMemStartMarker);
}

MemoryManager* MemoryManager::GetInstance() {
  if (instance == nullptr) {
    throw std::exception{Util::StrFormat(
        "MemoryManager::GetInstance => instance doesn't exist, make sure you "
        "access memory manager after its initialization")};
  }
  return instance;
}

void MemoryManager::DefragmentTest() {
  const U32 count = 1024;
  Array<ObjectHandle<U64>> arr;

  for (U32 i = 0; i < count; ++i) {
    auto ref = NewDynamic<U64>();
    *ref = i;
    arr.PushBack(ref);
  }

  auto map = instance->dynamicArena.addressIndexMap;

  for (U32 i = 0; i < count / 2; ++i) {
    int index = Math::Random::GetRandomGenerator(0, arr.Size() - 1).GetValue();
    DeleteDynamic(arr[index]);
    arr.Erase(arr.begin() + index);
  }
}
}  // namespace Isetta
