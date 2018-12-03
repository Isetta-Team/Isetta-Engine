/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <vector>
#include "Core/Debug/Assert.h"
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"
#include "MemUtil.h"
#include "SID/sid.h"

namespace Isetta {
/*
 * Requirements:
 * 1. Alloc: Go through the sorted (by address) list and find the first one that
 * fits size need
 * 2. Free: Add the pointer back to the list, and try to merge adjacent ones.
 * It's easy to find the next Node. Merging with last node can be done during
 * insertion
 * 3. Satisfy alignment requirement
 */
// TODO(YIDI): Optimize to use a red black tree as underlying structure to
// reduce time complexity
class ISETTA_API FreeListAllocator {
 public:
  // This class is using RAII
  FreeListAllocator() = delete;
  explicit FreeListAllocator(Size size);
  ~FreeListAllocator();

  void* Alloc(Size size, U8 alignment);
  void Free(void* memPtr);
  void* Realloc(void* memPtr, Size newSize, U8 alignment);

  template <typename T, typename... Args>
  T* New(Args... args);
  template <typename T>
  void Delete(T* t);
  template <typename T>
  T* NewArr(Size length, U8 alignment);
  template <typename T>
  void DeleteArr(Size length, T* ptrToDelete);

 private:
  struct Node {
    Size size;
    Node* next;
    explicit Node(const Size size) : size(size), next(nullptr) {}
  };

  struct AllocHeader {
    Size size;
    U64 adjustment;  // using U64 for alignment. U8 is big enough
    AllocHeader(const Size size, const U64 adjustment)
        : size(size), adjustment(adjustment) {}
  };

  void Expand();
  void RemoveNode(Node* last, Node* nodeToRemove);
  void InsertNode(Node* newNode);
  static void InsertNodeAt(Node* pos, Node* newNode);
  static void TryMergeWithNext(Node* node);

  Node* head = nullptr;
  void* memHead = nullptr;
  std::vector<void*> additionalMemory;

  // A lesson learned here: if these two variables are not static, it will
  // implicitly involve in the copy constructor's copying process. But as they
  // are const, they can't be copied over. This caused copy constructor to be
  // implicitly declared as deleted
  static const Size nodeSize = sizeof(Node);
  static const Size headerSize = sizeof(AllocHeader);

#if _DEBUG
  Size totalSize{0};
  Size sizeUsed{0};
  Size numOfNews{0};
  Size numOfDeletes{0};
  Size numOfArrNews{0};
  Size numOfArrDeletes{0};
  Size numOfAllocs{0};
  Size numOfFrees{0};
  using Allocations = std::pair<std::string, U16>;
  std::unordered_map<StringId, Allocations> monitor;
  std::unordered_map<U64, std::string> vtableToNameMap;
  bool monitorPureAlloc = true;
  void Print() const;
#endif

  friend class MemoryManager;
};

template <typename T, typename... Args>
T* FreeListAllocator::New(Args... args) {
#if _DEBUG
  numOfNews++;
  monitorPureAlloc = false;
  T* ret = new (Alloc(sizeof(T), MemUtil::ALIGNMENT)) T(args...);
  monitorPureAlloc = true;
  std::string name = MemUtil::GetNameForType<T>();
  if (std::is_base_of<class Component, T>::value) {
    U64 vPointer = *reinterpret_cast<U64*>(ret);
    vtableToNameMap.insert({vPointer, name});
  }

  StringId sid = SID(name.c_str());
  auto it = monitor.find(sid);
  if (it != monitor.end()) {
    Allocations allocations = it->second;
    allocations.second++;
    it->second = allocations;
  } else {
    monitor.insert({sid, {name, 1}});
  }
  return ret;
#else
  return new (Alloc(sizeof(T), MemUtil::ALIGNMENT)) T(args...);
#endif
}

template <typename T>
void FreeListAllocator::Delete(T* t) {
#if _DEBUG
  numOfDeletes++;
  std::string name;

  if (std::is_base_of<class Component, T>::value) {
    U64 vPointer = *reinterpret_cast<U64*>(t);
    name = vtableToNameMap.find(vPointer)->second;
  } else {
    name = MemUtil::GetNameForType<T>();
  }

  StringId sid = SID(name.c_str());
  auto it = monitor.find(sid);
  ASSERT(it != monitor.end());
  Allocations allocations = it->second;
  allocations.second--;
  if (allocations.second == 0) {
    monitor.erase(it);
  } else {
    it->second = allocations;
  }
  t->~T();

  monitorPureAlloc = false;
  Free(static_cast<void*>(t));
  monitorPureAlloc = true;
#else
  t->~T();
  Free(static_cast<void*>(t));
#endif
}

template <typename T>
T* FreeListAllocator::NewArr(Size length, const U8 alignment) {
  ASSERT(length != 0);
#if _DEBUG
  monitorPureAlloc = false;
  void* alloc = Alloc(sizeof(T) * length, alignment);
  monitorPureAlloc = true;

  numOfArrNews++;
  std::string name = MemUtil::GetNameForType<T>();
  if (std::is_base_of<class Component, T>::value) {
    U64 vPointer = *reinterpret_cast<U64*>(alloc);
    vtableToNameMap.insert({vPointer, name});
  }

  name += " Array[";
  name += std::to_string(length);
  name += "]";

  StringId sid = SID(name.c_str());
  auto it = monitor.find(sid);
  if (it != monitor.end()) {
    Allocations allocations = it->second;
    allocations.second++;
    it->second = allocations;
  } else {
    monitor.insert({sid, {name, 1}});
  }
#else
  void* alloc = Alloc(sizeof(T) * length, alignment);
#endif

  char* allocAddress = static_cast<char*>(alloc);
  for (Size i = 0; i < length; ++i) new (allocAddress + i * sizeof(T)) T;
  return static_cast<T*>(alloc);
}

template <typename T>
void FreeListAllocator::DeleteArr(const Size length, T* ptrToDelete) {
#if _DEBUG
  numOfArrDeletes++;
  std::string name;

  if (std::is_base_of<class Component, T>::value) {
    U64 vPointer = *reinterpret_cast<U64*>(ptrToDelete);
    name = vtableToNameMap.find(vPointer)->second;
  } else {
    name = MemUtil::GetNameForType<T>();
  }

  name += " Array[";
  name += std::to_string(length);
  name += "]";

  StringId sid = SID(name.c_str());
  auto it = monitor.find(sid);

  // ASSERT(it != monitor.end());
  if (it != monitor.end()) {
    Allocations allocations = it->second;
    allocations.second--;
    ASSERT(allocations.second >= 0);
    if (allocations.second == 0) {
      monitor.erase(it);
    } else {
      it->second = allocations;
    }
  }

  for (Size i = 0; i < length; ++i) ptrToDelete[i].~T();
  monitorPureAlloc = false;
  Free(static_cast<void*>(ptrToDelete));
  monitorPureAlloc = true;
#else
  for (Size i = 0; i < length; ++i) ptrToDelete[i].~T();
  Free(static_cast<void*>(ptrToDelete));
#endif
}

}  // namespace Isetta
