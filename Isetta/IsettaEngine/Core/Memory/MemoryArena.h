/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <map>
#include "Core/IsettaAlias.h"

namespace Isetta {

template <typename T>
class ObjectHandle;

class MemoryArena {
 private:
  MemoryArena() = delete;

  /**
   * \brief Create a new memory arena of specified size
   * \param size Size in byte
   */
  explicit MemoryArena(Size size);
  ~MemoryArena();

  /**
   * \brief Create a new object on this memory arena. You need
   * to manually call `DeleteDynamic` to free the memory of this object. The
   * dynamic memory area will be automatically defragmented so it's not a good
   * idea to create super big objects with this function
   *
   * \tparam T Type of the object you want to create
   * \return
   */
  template <typename T, typename ...args>
  ObjectHandle<T> NewDynamic(args...);

  /**
   * \brief Delete an object that was created with `NewDynamic`. The memory will
   * be freed and the constructor will be automatically called on the object
   *
   * \tparam T
   * \param objToDelete Object you want to delete
   */
  template <typename T>
  void DeleteDynamic(const ObjectHandle<T>& objToDelete);

  /**
   * \brief Helper function for allocate memory with Memory Arena
   * \param size Size in byte
   * \param outSize Returns the actual used size of memory (extra is possible
   * because of alignment requirement)
   * \return Pointer to memory
   */
  void* Alloc(Size size, Size& outSize);

  /**
   * \brief Defragment the memory arena. This should be called by the memory
   * manager
   */
  void Defragment();

  /**
   * \brief A helper function used to move a memory chunk to the left, eliminate
   * free space between it and the object immediately to its left
   * \param index
   * Index of the entry to move
   */
  void MoveLeft(U32 index);

  /// only for internal debugging
  void Print() const;

  /// only for internal use
  PtrInt GetUsedSize() const;

  // can't be put in ObjectHandle because it creates new ones for each type
  static const U32 maxHandleCount = 2048;
  static inline U32 nextUniqueID = 0;
  static class HandleEntry entryArr[];

  // TODO(YIDI): Make a custom container rather than using map
  // O(1) random access, O(log n) insert
  std::map<PtrInt, int> addressIndexMap;
  U32 curIndex = 0;
  PtrInt leftAddress{};
  PtrInt rightAddress{};
  void* memHead{};

  template <typename T>
  friend class ObjectHandle;
  friend class MemoryManager;
};

template <typename T, typename ...args>
ObjectHandle<T> MemoryArena::NewDynamic(args... argList) {
  // TODO(YIDI): I'm not sure this is the right way to implement object handles,
  // the handle itself should not need to be freed. But I think usage-wise this
  // is optimal
  Size size;
  void* mem = Alloc(sizeof(T), size);
  auto handle = ObjectHandle<T>{mem, nextUniqueID++, size, argList...};
  addressIndexMap.emplace(handle.GetObjAddress(), handle.index);
  return handle;
}

template <typename T>
void MemoryArena::DeleteDynamic(const ObjectHandle<T>& objToDelete) {
  auto addressIndexPair = addressIndexMap.find(objToDelete.GetObjAddress());

  if (addressIndexPair != addressIndexMap.end()) {
    addressIndexMap.erase(addressIndexPair);
    objToDelete.EraseObject();
  } else {
    throw std::exception{
        "MemoryArena::DeleteDynamic => Double deleting handle!"};
  }
}

}  // namespace Isetta
