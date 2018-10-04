/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Config/CVar.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/FreeListAllocator.h"
#include "Core/Memory/MemUtil.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {
// TODO(YIDI): Alloc all allocators on a big stack
class MemoryManager {
 public:
  struct MemoryConfig {
    CVar<Size> lsrAndLevelAllocatorSize{"LSR_and_level_allocator_size", 10_MB};
    CVar<Size> singleFrameAllocatorSize{"single_frame_allocator_size", 10_MB};
    CVar<Size> doubleBufferedAllocatorSize{"double_buffered_allocator_size",
                                           10_MB};
    CVar<Size> dynamicArenaSize{"dynamic_arena_size", 10_MB};
    CVar<Size> freeListAllocatorSize{"free_list_allocator_size", 10_MB};
  };

  /**
   * \brief Grab some properly aligned free memory to use. The memory will be
   * automatically freed at the end of this frame
   *
   * \param size Size in byte
   * \param alignment Alignment requirement of property, must be power of 2 and
   * less than 128, default is 16
   *
   * \return
   */
  static void* AllocOnSingleFrame(Size size, U8 alignment = MemUtil::ALIGNMENT);

  /**
   * \brief Create a new object whose memory is going to be cleared at the end
   * of this frame. You need to manually call destructor on that object if
   * needed
   *
   * \tparam T type of object to create
   * \return
   */
  template <typename T, typename... args>
  static T* NewOnSingleFrame(args...);

  template <typename T>
  static T* NewArrOnSingleFrame(Size length, U8 alignment = MemUtil::ALIGNMENT);

  /**
   * \brief Grab some properly aligned free memory to use. The memory will be
   * automatically freed at the end of the next frame
   *
   * \param size Size in byte
   * \param alignment Alignment requirement of property, must be power of 2 and
   * less than 256, default is 16
   *
   * \return
   */
  static void* AllocOnDoubleBuffered(Size size,
                                     U8 alignment = MemUtil::ALIGNMENT);

  /**
   * \brief Create a new object whose memory is going to be cleared at the end
   * of the next frame. You need to manually call destructor on that object if
   * needed
   *
   * \tparam T type of object to create \return
   */
  template <typename T, typename... args>
  static T* NewOnDoubleBuffered(args...);

  template <typename T>
  static T* NewArrOnDoubleBuffered(Size length,
                                   U8 alignment = MemUtil::ALIGNMENT);

  // the memory will either from LSR - when starting up the engine
  // or LevelData - after engine startup
  // make sure you don't put data in wrong places
  static void* AllocOnStack(Size size, U8 alignment = MemUtil::ALIGNMENT);

  template <typename T, typename... args>
  static T* NewOnStack(args...);

  template <typename T>
  static T* NewArrOnStack(Size length, U8 alignment = MemUtil::ALIGNMENT);

  // TODO(YIDI): Use different freelist allocators for different stage
  static void* AllocOnFreeList(Size size, U8 alignment = MemUtil::ALIGNMENT);

  template <typename T, typename... args>
  static T* NewOnFreeList(args... argList);

  template <typename T>
  static T* NewArrOnFreeList(Size length, U8 alignment = MemUtil::ALIGNMENT);

  static void FreeOnFreeList(void* memPtr);

  /**
   * \brief Create an object that will sit on the dynamic memory area. You need
   * to manually call `DeleteDynamic` to free the memory of this object. The
   * dynamic memory area will be automatically defragmented so it's not a good
   * idea to create super big objects with this function
   *
   * \tparam T Type of the object you want to create
   * \return
   */
  template <typename T, typename... args>
  static ObjectHandle<T>& NewDynamic(args...);

  /**
   * \brief Delete an object that was created with `NewDynamic`. The memory will
   * be freed and the constructor will be automatically called on the object
   *
   * \tparam T
   * \param objToDelete Object you want to delete
   */
  template <typename T>
  static void DeleteDynamic(const ObjectHandle<T>& objToDelete);

 private:
  MemoryManager();
  ~MemoryManager() = default;

  /**
   * \brief Start up the memory manager. This creates the single frame
   * allocator, the double buffered allocator, and the dynamic arena. Their
   * specific sizes can be specified in engine config
   */
  void StartUp();

  /**
   * \brief Update the memory manager. This needs to be called in simulation
   * update, and takes care of clearing and swapping single frame + double
   * buffered allocators and defragment the dynamic memory arena
   */
  void Update();

  /**
   * \brief Free all memory, any further attempt to use objects managed by the
   * memory manager will crash the game
   */
  void ShutDown();

  // set the marker to clear to when finishing a level
  // TODO(YIDI): Someone please call this
  void FinishEngineStartupListener();
  // TODO(YIDI): Someone please call this
  void ClearLevelMemory();

  static MemoryManager* GetInstance();
  /// only for internal test
  void RegisterTests();
  /// only for internal test
  static void DefragmentTest();

  static MemoryManager* instance;
  StackAllocator lsrAndLevelAllocator{};
  Size lvlMemStartMarker{};
  StackAllocator singleFrameAllocator{};
  DoubleBufferedAllocator doubleBufferedAllocator{};
  MemoryArena dynamicArena{};
  FreeListAllocator freeListAllocator{};

  friend class EngineLoop;
};

template <typename T, typename... args>
T* MemoryManager::NewOnSingleFrame(args... argList) {
  return GetInstance()->singleFrameAllocator.New<T>(argList...);
}

template <typename T>
T* MemoryManager::NewArrOnSingleFrame(const Size length, const U8 alignment) {
  return GetInstance()->singleFrameAllocator.NewArr<T>(length, alignment);
}

template <typename T, typename... args>
T* MemoryManager::NewOnDoubleBuffered(args... argList) {
  return GetInstance()->doubleBufferedAllocator.New<T>(argList...);
}

template <typename T>
T* MemoryManager::NewArrOnDoubleBuffered(const Size length,
                                         const U8 alignment) {
  return GetInstance()->doubleBufferedAllocator.NewArr<T>(length, alignment);
}

template <typename T, typename... args>
T* MemoryManager::NewOnStack(args... argList) {
  return GetInstance()->lsrAndLevelAllocator.New<T>(argList...);
}

template <typename T>
T* MemoryManager::NewArrOnStack(const Size length, const U8 alignment) {
  return GetInstance()->lsrAndLevelAllocator.NewArr<T>(length, alignment);
}

template <typename T, typename... args>
T* MemoryManager::NewOnFreeList(args... argList) {
  return GetInstance()->freeListAllocator.New<T>(argList...);
}

template <typename T>
T* MemoryManager::NewArrOnFreeList(const Size length, const U8 alignment) {
  return GetInstance()->freeListAllocator.NewArr<T>(length, alignment);
}

template <typename T, typename... args>
ObjectHandle<T>& MemoryManager::NewDynamic(args... argList) {
  return GetInstance()->dynamicArena.NewDynamic<T>(argList...);
}

template <typename T>
void MemoryManager::DeleteDynamic(const ObjectHandle<T>& objToDelete) {
  GetInstance()->dynamicArena.DeleteDynamic(objToDelete);
}

}  // namespace Isetta
