#pragma once
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"

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
};

template <typename T>
class ObjectHandle {
 public:
  ~ObjectHandle();
  T* operator->() const;
  T& operator*();

 private:
  explicit ObjectHandle();
  T* GetObjectPtr() const;

  U32 uniqueID;
  U32 index;
  inline static U32 nextID = 0;
  static const int maxTableSize = 2048;
  inline static HandleEntry table[maxTableSize];

  friend class MemoryManager;
};

template <typename T>
ObjectHandle<T>::ObjectHandle() : uniqueID(nextID++) {
  HandleEntry* entry = nullptr;

  for (int i = 0; i < maxTableSize; i++) {
    if (table[i].isEmpty) {
      index = i;
      entry = &table[i];
      break;
    }
  }

  if (entry == nullptr) {
    throw std::out_of_range{"No empty slot in handle table"};
  }

  // TODO(YIDI): use our own memory mgt
  T* t = new T{};
  entry->Set(uniqueID, static_cast<void*>(t), false);
}

template <typename T>
ObjectHandle<T>::~ObjectHandle() {
  HandleEntry entry = table[index];
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
  HandleEntry entry = table[index];

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

}  // namespace Isetta
