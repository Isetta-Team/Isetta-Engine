/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {

// TODO(YIDI): Add LSR
// TODO(YIDI): Implement de-fragmentation
class MemoryManager {
 public:
  template <typename T>
  T* NewSingleFrame();

  void* AllocSingleFrameUnAligned(Size size);
  void* AllocSingleFrame(Size size, U8 alignment = 16);

  template <typename T>
  static T* NewDoubleBuffered();

  void* AllocDoubleBufferedUnAligned(Size size);
  void* AllocDoubleBuffered(Size size, U8 alignment = 16);

  template <typename T>
  static ObjectHandle<T>& NewDynamic();

  template <typename T>
  static void DeleteDynamic(const ObjectHandle<T>& objToFree);

  static void Test();

 private:
  MemoryManager();
  ~MemoryManager() = default;

  void StartUp();
  void Update();
  void ShutDown();
  void RegisterCallbacks();

  static MemoryManager* instance;
  StackAllocator singleFrameAllocator{};
  DoubleBufferedAllocator doubleBufferedAllocator{};
  MemoryArena dynamicArena{};

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
  return instance->dynamicArena.NewDynamic<T>();
}

template <typename T>
void MemoryManager::DeleteDynamic(const ObjectHandle<T>& objToFree) {
  instance->dynamicArena.DeleteDynamic(objToFree);
}

}  // namespace Isetta
