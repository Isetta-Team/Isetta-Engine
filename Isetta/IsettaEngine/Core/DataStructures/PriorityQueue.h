/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <initializer_list>
#include "Core/DataStructures/Array.h"
#include "ISETTA_API.h"

namespace Isetta {
template <typename T>
struct ISETTA_API_DECLARE Less {
  bool operator()(const T& a, const T& b) { return a < b; }
};

template <typename T>
struct ISETTA_API_DECLARE Greater {
  bool operator()(const T& a, const T& b) { return a > b; }
};

template <typename T, template <typename> class compare = Less>
class ISETTA_API_DECLARE PriorityQueue {
 private:
  Array<T> elements;

  void Heapify(int);
  inline int Parent(int i) { return 0.5 * (i - 1); }
  inline int Left(int i) { return 2 * i; }
  inline int Right(int i) { return 2 * i + 1; }
  inline void PushToTop(int i, int parent);

 public:
  PriorityQueue() = default;
  PriorityQueue(const std::initializer_list<T>& list) : elements{list} {}
  PriorityQueue(const Array<T>& elements) : elements{elements} {}
  PriorityQueue(const T* beginPtr, const T* endPtr)
      : elements{beginPtr, endPtr} {}

  ~PriorityQueue() = default;

  PriorityQueue(const PriorityQueue& inQueue) { elements = inQueue.elements; }
  PriorityQueue(PriorityQueue&& inQueue) noexcept {
    elements = std::move(inQueue.elements);
  }
  inline PriorityQueue& operator=(const PriorityQueue& inQueue) {
    elements = inQueue.elements;
    return *this;
  }
  inline PriorityQueue& operator=(PriorityQueue&& inQueue) noexcept {
    elements = std::move(inQueue.elements);
    return *this;
  }

  inline void Clear();
  template <typename... Args>
  inline void Emplace(Args&&... args);
  inline bool IsEmpty() const { return elements.Size() == 0; }
  inline void Pop();
  inline void Push(const T& val);
  inline void Push(T&& val);
  inline int Size() const { return elements.Size(); }
  inline void Swap(PriorityQueue& inQueue) noexcept;
  const T& Top() const;
};
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::Heapify(int i) {
  int l = Left(i);
  int r = Right(i);
  int min = i;
  if (l < elements.Size() && compare<T>()(elements[l], elements[min])) min = l;
  if (r < elements.Size() && compare<T>()(elements[r], elements[min])) min = r;
  if (min != i) {
    T tmp = elements[i];
    elements[i] = elements[min];
    elements[min] = tmp;
    Heapify(i);
  }
}
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::PushToTop(int i, int parent) {
  while (i != 0 && compare<T>()(elements[i], elements[parent])) {
    T tmp = elements[i];
    elements[i] = elements[parent];
    elements[parent] = tmp;
    i = Parent(i);
    parent = Parent(i);
  }
}
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::Clear() {
  elements.Clear();
}
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::Pop() {
  elements.Erase(elements.begin());
  Heapify(0);
}
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::Push(const T& val) {
  elements.PushBack(val);
  int i = elements.Size() - 1;
  PushToTop(i, Parent(i));
}
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::Push(T&& val) {
  int i = elements.Size();
  elements.PushBack(val);
  PushToTop(i, Parent(i));
}
template <typename T, template <typename> class compare>
inline void PriorityQueue<T, compare>::Swap(PriorityQueue& inQueue) noexcept {
  elements.Swap(inQueue.elements);
}
template <typename T, template <typename> class compare>
inline const T& PriorityQueue<T, compare>::Top() const {
  return elements[0];
}
template <typename T, template <typename> class compare>
template <typename... Args>
inline void PriorityQueue<T, compare>::Emplace(Args&&... args) {
  int i = elements.Size();
  elements.EmplaceBack(std::forward<Args>(args)...);
  PushToTop(i, Parent(i));
}
}  // namespace Isetta
