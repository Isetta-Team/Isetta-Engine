/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/StackAllocator.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Debug/Logger.h"

namespace Isetta {

template<typename T>
class ObjectHandle;

class MemoryManager {
 public:
  template <typename T>
  T* NewSingleFrame();

  template <typename T>
  static T* NewDoubleBuffered();

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

  StackAllocator singleFrameAllocator;
  DoubleBufferedAllocator doubleBufferedAllocator;

  // these can't be put in ObjectHandle because it creates new ones for each type
  inline static U32 nextUniqueID = 0;
  const static U32 maxTableSize = 2048;
  static class HandleEntry handleTable[];

  template<typename T>
  friend class ObjectHandle;
};

template <typename T>
ObjectHandle<T>& MemoryManager::NewDynamic() {
  // TODO(YIDI): use a pool to manage object handles
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
