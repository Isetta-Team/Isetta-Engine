/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryManager.h"

namespace Isetta {
class HandleEntry {
 private:
  HandleEntry() = default;
  ~HandleEntry() = default;

  void Set(U32 uniqueID, void* ptr, bool isEmpty);

  U32 uniqueID{};
  void* ptr{};
  bool isEmpty{true};

  template <typename T>
  friend class ObjectHandle;
  friend class MemoryManager;
};

template <typename T>
class ObjectHandle {
 public:
  ~ObjectHandle() = default;
  T* operator->() const;
  T& operator*();

 private:
  ObjectHandle();
  T* GetObjectPtr() const;
  void EraseObject() const;
  U32 uniqueID;
  U32 index;

  friend class MemoryManager;
};

template <typename T>
ObjectHandle<T>::ObjectHandle() {
  uniqueID = MemoryManager::nextUniqueID++;
  HandleEntry* entry = nullptr;

  for (U32 i = 0; i < MemoryManager::maxTableSize; i++) {
    if (MemoryManager::handleEntryTable[i].isEmpty) {
      index = i;
      entry = &MemoryManager::handleEntryTable[index];
      break;
    }
  }

  if (entry == nullptr) {
    throw std::out_of_range{"No empty slot in handle table"};
  }

  // TODO(YIDI): use our own memory mgt
  T* t = new T{};
  void* ptr = static_cast<void*>(t);
  entry->Set(uniqueID, ptr, false);
}

template <typename T>
T* ObjectHandle<T>::operator->() const {
  return GetObjectPtr();
}

template <typename T>
T& ObjectHandle<T>::operator*() {
  return *GetObjectPtr();
}

template <typename T>
T* ObjectHandle<T>::GetObjectPtr() const {
  HandleEntry entry = MemoryManager::handleEntryTable[index];

  if (entry.isEmpty) {
    throw std::exception{"ObjectHandle::GetObject() : Object already deleted"};
  }

  // prevent the problem of "stale pointer"
  if (uniqueID == entry.uniqueID) {
    return static_cast<T*>(entry.ptr);
  }

  throw std::exception{
      "ObjectHandle::GetObject() : Object you are trying to access was "
      "replaced by a new object"};
}

template <typename T>
void ObjectHandle<T>::EraseObject() const {
  HandleEntry entry = MemoryManager::handleEntryTable[index];

  if (entry.isEmpty) {
    // TODO(YIDI): Is this a good use of log_error?
    LOG_ERROR(Debug::Channel::Memory,
              "ObjectHandle::DeleteObject() : Double deleting handle!");
    return;
  }

  // TODO(YIDI): Use our own memory mgt
  delete static_cast<T*>(entry.ptr);
  entry.isEmpty = true;
}

}  // namespace Isetta
