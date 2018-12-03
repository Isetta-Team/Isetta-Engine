/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <cmath>
#include <initializer_list>
#include "Core/Memory/MemoryManager.h"
#include "ISETTA_API.h"
#include "Core/DataStructures/Array.h"

namespace Isetta {
template <typename T>
class ISETTA_API_DECLARE RingBuffer {
 public:
  // Constructors
  RingBuffer();
  explicit RingBuffer(int n);
  RingBuffer(std::initializer_list<T> il);
  explicit RingBuffer(std::initializer_list<T> il, int n);
  ~RingBuffer() { MemoryManager::DeleteArrOnFreeList<T>(size, buffer); }

  // Copy and move constructors
  RingBuffer(const RingBuffer<T>& rb);
  RingBuffer(RingBuffer<T>&& rb) noexcept;
  RingBuffer<T>& operator=(const RingBuffer<T>& rb);

  // Accessors and mutators
  T Get();
  void Put(T o);
  T GetBack();
  void PutFront(T o);
  void Clear() { head = tail; }
  Array<T> ToArray() const;

  // Property accessors

  bool IsEmpty() const { return head == tail; }
  bool IsFull() const { return (tail + 1) % size == head; }

  int GetCapacity() const { return size - 1; }
  int GetLength() const { return abs(head - tail); }

 private:
  T* buffer;
  int head = 0;
  int tail = 0;
  int size = 64;  // The buffer allows usage of size - 1 slots so that we
                  // can test for fullness without checking every put/get
  // The final number of usable slots is equivalent to the user's passed size
  // to the constructor size
  // Also cannot be unsigned because it is used for negative modulus calculation
};

template <typename T>
RingBuffer<T>::RingBuffer() {
  buffer = MemoryManager::NewArrOnFreeList<T>(size);
}

template <typename T>
RingBuffer<T>::RingBuffer(int n) : size{n + 1} {
  if (size <= 0) {
    throw std::length_error{
        "RingBuffer::RingBuffer => Cannot create RingBuffer of size 0 or "
        "less."};
  }

  buffer = MemoryManager::NewArrOnFreeList<T>(size);
}

template <typename T>
RingBuffer<T>::RingBuffer(std::initializer_list<T> il)
    : size{static_cast<int>(il.size()) + 1} {
  if (size <= 0) {
    throw std::length_error{
        "RingBuffer::RingBuffer => Cannot create RingBuffer of size 0 or "
        "less."};
  }

  buffer = MemoryManager::NewArrOnFreeList<T>(size);
  for (auto i : il) {
    Put(i);
  }
}

template <typename T>
RingBuffer<T>::RingBuffer(std::initializer_list<T> il, int n) : size{n + 1} {
  if (size <= 0) {
    throw std::length_error{
        "RingBuffer::RingBuffer => Cannot create RingBuffer of size 0 or "
        "less."};
  } else if (size < il.size()) {
    throw std::length_error{
        "RingBuffer::RingBuffer => Cannot create RingBuffer of size less than "
        "the passed in list."};
  }

  buffer = MemoryManager::NewArrOnFreeList<T>(size);
  for (auto i : il) {
    Put(i);
  }
}

template <typename T>
RingBuffer<T>::RingBuffer(const RingBuffer<T>& rb) : size(rb.GetCapacity()) {
  buffer = MemoryManager::NewArrOnFreeList<T>(size);
  for (auto it : rb.ToArray()) {
    Put(it);
  }
}

template <typename T>
RingBuffer<T>& RingBuffer<T>::operator=(const RingBuffer<T>& rb) {
  if (this == &rb) {
    return *this;
  }

  if (buffer) {
    MemoryManager::DeleteArrOnFreeList<T>(size, buffer);
  }
  size = rb.GetCapacity();
  buffer = MemoryManager::NewArrOnFreeList<T>(size);

  for (auto it : rb.ToArray()) {
    Put(it);
  }

  return *this;
}

template <typename T>
RingBuffer<T>::RingBuffer(RingBuffer<T>&& rb) noexcept : size{rb.GetCapacity()} {
  buffer = MemoryManager::NewArrOnFreeList<T>(size);

  while (!rb.IsEmpty()) {
    Put(rb.Get());
  }
}

template <typename T>
T RingBuffer<T>::Get() {
  if (IsEmpty()) {
    throw std::out_of_range{"RingBuffer::get => Buffer is empty."};
  }

  int idx = head;
  head = (head + 1) % size;

  return buffer[idx];
}

template <typename T>
void RingBuffer<T>::Put(T o) {
  if (IsFull()) {
    throw std::range_error{"RingBuffer::put => Buffer is full."};
  }

  buffer[tail] = o;
  tail = (tail + 1) % size;
}

template <typename T>
T RingBuffer<T>::GetBack() {
  if (IsEmpty()) {
    throw std::out_of_range{"RingBuffer::getBack => Buffer is empty."};
  }

  tail = std::abs(static_cast<int>((tail - 1) % size));

  return buffer[tail];
}

template <typename T>
void RingBuffer<T>::PutFront(T o) {
  if (IsFull()) {
    throw std::out_of_range{"RingBuffer::putBack => Buffer is full."};
  }

  head = ((head - 1) % size + size) % size;
  buffer[head] = o;
}

template <typename T>
Array<T> RingBuffer<T>::ToArray() const {
  Array<T> arr;
  int count = 0;
  int idx = head;
  while (idx != tail) {
    arr[count] = buffer[idx];
    idx = (idx + 1) % size;
    ++count;
  }
  return arr;
}

}  // namespace Isetta
