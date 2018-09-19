/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryArena.h"

namespace Isetta {
// FUTURE: If we want reference counting (probably not), it can be done here
class HandleEntry {
 private:
  HandleEntry() = default;
  ~HandleEntry() = default;
  PtrInt GetAddress() const;

  // just a help function
  void Set(U32 uniqueID, void* ptr, bool isEmpty, SizeInt size);

  U32 uniqueID{};
  SizeInt size{};
  mutable void* ptr{};
  bool isEmpty{true};

  template <typename T>
  friend class ObjectHandle;
  friend class MemoryArena;
};

template <typename T>
class ObjectHandle {
 public:
  ObjectHandle() = default;
  ~ObjectHandle() = default;
  T* operator->() const;
  T& operator*();

  ObjectHandle(void* mem, U32 uniqueID, SizeInt size);

  T* GetObjectPtr() const;
  void EraseObject() const;
  PtrInt GetObjAddress() const;
  U32 uniqueID;
  U32 index;
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
ObjectHandle<T>::ObjectHandle(void* mem, const U32 uniqueID, const SizeInt size): uniqueID(uniqueID) {
  HandleEntry* entry = nullptr;

  // FUTURE: To optimize the speed of this, remember last index
  for (U32 i = 0; i < MemoryArena::maxHandleCount; i++) {
    if (MemoryArena::entryArr[i].isEmpty) {
      index = i;
      entry = &MemoryArena::entryArr[index];
      break;
    }
  }

  if (entry == nullptr) {
    throw std::out_of_range{
      "ObjectHandle => No empty slot in handle table"
    };
  }

  T* t = new(mem) T{};
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
        "ObjectHandle::DeleteObject => Double deleting handle!"};
  }

  if (uniqueID != entry.uniqueID) {
    throw std::exception{
        "ObjectHandle::DeleteObject => You are trying to delete an "
        "object you don't own!"};
  }

  static_cast<T*>(entry.ptr)->~T();
  entry.isEmpty = true;
}

template <typename T>
PtrInt ObjectHandle<T>::GetObjAddress() const {
  return reinterpret_cast<PtrInt>(GetObjectPtr());
}

}  // namespace Isetta
