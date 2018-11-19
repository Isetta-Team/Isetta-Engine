/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryArena.h"

namespace Isetta {
// FUTURE: If we want reference counting (probably not), it can be done here
/**
 * \brief Individual entries in the global handle table. Stores some information
 * about the object pointed to
 */
class HandleEntry {
 private:
  /**
   * \brief Default constructor that does nothing
   */
  HandleEntry() = default;

  /**
   * \brief Default destructor that does nothing
   */
  ~HandleEntry() = default;

  /**
   * \brief A helper function that returns the address of pointed object
   * \return Address in type PtrInt
   */
  PtrInt GetAddress() const;

  /**
   * \brief Just a help function for setting individual fields of the handle
   * entry
   *
   * \param uniqueID
   * \param ptr
   * \param isEmpty
   * \param size
   */
  void Set(U32 uniqueID, void* ptr, bool isEmpty, Size size);

  /// unique ID. Always compare this with the uniqueID of ObjectHandle used to
  /// manipulate this entry
  U32 uniqueID{};

  /// Size of the object pointed to by this entry
  Size size{};

  /// Pointer to the actual object's address. Marked as mutable because its
  /// moved in the defragmentation process
  mutable void* ptr{};

  /// Is this entry empty and can be used for a new object?
  bool isEmpty{true};

  template <typename T>
  friend class ObjectHandle;
  friend class MemoryArena;
};

template <typename T>
class ObjectHandle {
 public:
  /**
   * \brief Default constructor that does nothing. Exists to support having an
   * array of ObjectHandles.
   */
  ObjectHandle() = default;

  /**
   * \brief Default destructor that does nothing (cause there is nothing to do).
   */
  ~ObjectHandle() = default;

  /**
   * \brief The actual constructor that the MemoryArena uses to create new
   * ObjectHandles \param mem Pointer to the memory \param uniqueID uniqueID for
   * this handle \param size Size of the object in bytes
   */
  template <typename... args>
  ObjectHandle(void* mem, U32 uniqueID, Size size, args...);

  /**
   * \brief Returns a pointer to the actual object
   * \return A pointer to the actual object
   */
  T* operator->() const;

  /**
   * \brief Returned the dereferenced object
   * \return A reference to the object
   */
  T& operator*();

  explicit operator bool() const;

 private:
  /**
   * \brief Helper function for finding the pointer the actual object
   * \return
   */
  T* GetObjectPtr() const;

  /**
   * \brief Erase the object. This frees the memory, frees the corresponding
   * handle entry, and calls destructor on the object
   */
  void EraseObject() const;

  /**
   * \brief A helper function used to get memory address of the object pointed
   * to by this handle
   *
   * \return Memory address of the object pointed to by this
   * handle
   */
  PtrInt GetObjAddress() const;
  U32 uniqueID{U32_MAX};
  U32 index{U32_MAX};

  friend class MemoryArena;
};

template <typename T>
T* ObjectHandle<T>::operator->() const {
  return GetObjectPtr();
}

template <typename T>
T& ObjectHandle<T>::operator*() {
  return *GetObjectPtr();
}

template <typename T>
ObjectHandle<T>::operator bool() const {
  if (index > MemoryArena::maxHandleCount) {
    return false;
  }

  HandleEntry& entry = MemoryArena::entryArr[index];

  return !entry.isEmpty && uniqueID == entry.uniqueID;
}

template <typename T>
template <typename... args>
ObjectHandle<T>::ObjectHandle(void* mem, const U32 uniqueID, const Size size,
                              args... argList)
    : uniqueID(uniqueID) {
  HandleEntry* entry = nullptr;

  // FUTURE: To optimize the speed of this, remember last index
  for (U32 i = 0; i < MemoryArena::maxHandleCount; ++i) {
    if (MemoryArena::entryArr[i].isEmpty) {
      index = i;
      entry = &MemoryArena::entryArr[index];
      break;
    }
  }

  if (entry == nullptr) {
    throw std::out_of_range{
        "ObjectHandle::ObjectHandle => No empty slot in handle table"};
  }

  T* t = new (mem) T{argList...};
  entry->Set(uniqueID, static_cast<void*>(t), false, size);
}

template <typename T>
T* ObjectHandle<T>::GetObjectPtr() const {
  HandleEntry& entry = MemoryArena::entryArr[index];

  if (entry.isEmpty) {
    throw std::exception{
        "ObjectHandle::GetObjectPtr => Object already deleted"};
  }

  // prevent the problem of "stale pointer"
  if (uniqueID != entry.uniqueID) {
    throw std::exception{
        "ObjectHandle::GetObjectPtr => Object you are trying to access was "
        "replaced by a new object"};
  }

  return static_cast<T*>(entry.ptr);
}

template <typename T>
void ObjectHandle<T>::EraseObject() const {
  HandleEntry& entry = MemoryArena::entryArr[index];

  if (entry.isEmpty) {
    throw std::exception{
        "ObjectHandle::EraseObject => ObjectHandle::DeleteObject => Double "
        "deleting handle!"};
  }

  if (uniqueID != entry.uniqueID) {
    throw std::exception{
        "ObjectHandle::EraseObject => ObjectHandle::DeleteObject => You are "
        "trying to delete an object you don't own!"};
  }

  static_cast<T*>(entry.ptr)->~T();
  entry.isEmpty = true;
}

template <typename T>
PtrInt ObjectHandle<T>::GetObjAddress() const {
  return reinterpret_cast<PtrInt>(GetObjectPtr());
}

}  // namespace Isetta
