/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/PoolAllocator.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {

template <typename T>
class ObjectHandle;

// TODO(YIDI): Add LSR
// TODO(YIDI): Implement de-fragmentation
class MemoryManager {
 public:
  template <typename T>
  T* NewSingleFrame();

  void* AllocSingleFrameUnAligned(SizeInt size);
  void* AllocSingleFrame(SizeInt size, U8 alignment = 16);

  template <typename T>
  static T* NewDoubleBuffered();

  void* AllocDoubleBufferedUnAligned(SizeInt size);
  void* AllocDoubleBuffered(SizeInt size, U8 alignment = 16);

  template <typename T>
  static ObjectHandle<T>& NewDynamic();

  template <typename T>
  static void DeleteDynamic(ObjectHandle<T>& objToFree);

 private:
  MemoryManager();
  ~MemoryManager() = default;

  void StartUp();
  void Update();
  void ShutDown();
  void Defragment();

  static void* AllocDynamic(SizeInt size, U8 alignment = 16);
  static void FreeDynamic(void* ptrToFree);

  static MemoryManager* instance;
  StackAllocator singleFrameAllocator{};
  StackAllocator dynamicArena{};
  DoubleBufferedAllocator doubleBufferedAllocator{};
  PoolAllocator handlePool;

  // can't be put in ObjectHandle because it creates new ones for each type
  inline static U32 nextUniqueID = 0;
  const static U32 maxTableSize = 2048;
  static class HandleEntry handleEntryTable[];
  inline static void* handleLoopUp[maxTableSize] = {nullptr};

  template <typename T>
  friend class ObjectHandle;
  friend class ModuleManager;
};

template <typename T>
T* MemoryManager::NewSingleFrame() {
  return singleFrameAllocator.New<T>();
}

template <typename T>
T* MemoryManager::NewDoubleBuffered() {
  return doubleBufferedAllocator.New<T>();
}

template <typename T>
ObjectHandle<T>& MemoryManager::NewDynamic() {
  // TODO(YIDI): I'm not sure this is the right way to implement object handles,
  // the handle itself should not need to be freed. But I think usage-wise this
  // is optimal
  auto hand = new (instance->handlePool.Get()) ObjectHandle<T>{};
  instance->handleLoopUp[hand->index] = hand;
  return *(hand);
}

template <typename T>
void MemoryManager::DeleteDynamic(ObjectHandle<T>& objToFree) {
  if (instance->handleLoopUp[objToFree.index] != nullptr) {
    objToFree.EraseObject();
    instance->handlePool.Free(instance->handleLoopUp[objToFree.index]);
    instance->handleLoopUp[objToFree.index] = nullptr;
  } else {
    LOG_ERROR(Debug::Channel::Memory, "Double deleting handle!");
  }
}

}  // namespace Isetta
