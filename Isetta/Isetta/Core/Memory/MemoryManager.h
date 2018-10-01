/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Config/CVar.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/DoubleBufferedAllocator.h"
#include "Core/Memory/ObjectHandle.h"
#include "Core/Memory/StackAllocator.h"

namespace Isetta {

class MemoryManager {
 public:
  struct MemoryConfig {
    CVar<Size> lsrAndLevelAllocatorSize{"LSR_and_level_allocator_size", 10_MB};
    CVar<Size> singleFrameAllocatorSize{"single_frame_allocator_size", 10_MB};
    CVar<Size> doubleBufferedAllocatorSize{"double_buffered_allocator_size",
                                           10_MB};
    CVar<Size> dynamicArenaSize{"dynamic_arena_size", 10_MB};
  };

  /**
   * \brief Create a new object whose memory is going to be cleared at the end
   * of this frame. You need to manually call destructor on that object if
   * needed
   *
   * \tparam T type of object to create
   * \return
   */
  template <typename T, typename ...args>
  static T* NewOnSingleFrame(args...);

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
  static void* AllocOnSingleFrame(Size size, U8 alignment = 16);

  /**
   * \brief Create a new object whose memory is going to be cleared at the end
   * of the next frame. You need to manually call destructor on that object if
   * needed
   *
   * \tparam T type of object to create \return
   */
  template <typename T, typename ...args>
  static T* NewOnDoubleBuffered(args...);

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
  static void* AllocOnDoubleBuffered(Size size, U8 alignment = 16);

  // the memory will either from LSR - when starting up the engine
  // or LevelData - after engine startup
  // make sure you don't put data in wrong places
  static void* AllocOnStack(Size size, U8 alignment = 16);

  /**
   * \brief Create an object that will sit on the dynamic memory area. You need
   * to manually call `DeleteDynamic` to free the memory of this object. The
   * dynamic memory area will be automatically defragmented so it's not a good
   * idea to create super big objects with this function
   *
   * \tparam T Type of the object you want to create
   * \return
   */
  template <typename T, typename ...args>
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
  void FinishEngineStartupListener();
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

  friend class ModuleManager;
};

template <typename T, typename ...args>
T* MemoryManager::NewOnSingleFrame(args... argList) {
  return singleFrameAllocator.New<T>(argList...);
}

template <typename T, typename ...args>
T* MemoryManager::NewOnDoubleBuffered(args... argList) {
  return doubleBufferedAllocator.New<T>(argList...);
}

template <typename T, typename ...args>
ObjectHandle<T>& MemoryManager::NewDynamic(args... argList) {
  return instance->dynamicArena.NewDynamic<T>(argList...);
}

template <typename T>
void MemoryManager::DeleteDynamic(const ObjectHandle<T>& objToDelete) {
  instance->dynamicArena.DeleteDynamic(objToDelete);
}

}  // namespace Isetta
