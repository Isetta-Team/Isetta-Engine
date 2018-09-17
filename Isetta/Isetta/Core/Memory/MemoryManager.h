/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {

template <typename T>
class ObjectHandle;

// TODO(YIDI): Add LSR
// TODO(YIDI): Add PoolAllocators
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
  static void FreeDynamic(ObjectHandle<T>& objToFree);

 private:
  MemoryManager();
  ~MemoryManager() = default;

  void StartUp();
  void Update();
  void ShutDown();

  StackAllocator singleFrameAllocator{};
  DoubleBufferedAllocator doubleBufferedAllocator{};

  // can't be put in ObjectHandle because it creates new ones for each type
  inline static U32 nextUniqueID = 0;
  const static U32 maxTableSize = 2048;
  static class HandleEntry handleTable[];

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
  // TODO(YIDI): use a pool to manage object handles
  // TODO(YIDI): I'm not sure this is the right way to implement object handles,
  // the handle itself should not need to be freed
  auto hand = new ObjectHandle<T>{};
  return *(hand);
}

template <typename T>
void MemoryManager::FreeDynamic(ObjectHandle<T>& objToFree) {
  objToFree.EraseObject();
  // TODO(YIDI): Use a pool allocator for this
  delete &objToFree;
}

}  // namespace Isetta
