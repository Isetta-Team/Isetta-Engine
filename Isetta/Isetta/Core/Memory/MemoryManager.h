#pragma once
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {

class MemoryManager {
 public:
  template <typename T>
  T* NewSingleFrame();

  template <typename T>
  static T* NewDoubleBuffered();

  template <typename T>
  static ObjectHandle<T> NewDynamic();

 private:
  explicit MemoryManager() = default;
  ~MemoryManager() = default;

  void StartUp();
  void Update();
  void ShutDown();

  StackAllocator singleFrameAllocator;
  DoubleBufferedAllocator doubleBufferedAllocator;
};

template <typename T>
ObjectHandle<T> MemoryManager::NewDynamic() {
  return ObjectHandle<T>{};
}

}  // namespace Isetta
