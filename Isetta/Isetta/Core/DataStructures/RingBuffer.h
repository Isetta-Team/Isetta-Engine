/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <initializer_list>
#include <stdexcept>
#include <cmath>

namespace Isetta {
template <typename T>
class RingBuffer {
 public:
  // Constructors

  // TODO(Caleb): Update for the custom mem alloc
  RingBuffer() : buffer(new T[size]) {}
  explicit RingBuffer(int n);
  explicit RingBuffer(std::initializer_list<T> il);
  explicit RingBuffer(std::initializer_list<T> il, int n);
  ~RingBuffer() {
    delete buffer;
  }

  // Copy and move constructors

  RingBuffer(const RingBuffer& rb);
  RingBuffer(RingBuffer&& rb) noexcept;

  // Accessors and mutators

  T Get();
  void Put(T o);
  T GetBack();
  void PutFront(T o);
  void Clear();
  T* ToList();

  // Property accessors

  bool IsEmpty() const;
  bool IsFull() const;
  int GetCapacity() const;
  int GetLength() const;

 private:
  T* buffer;
  int head = 0;
  int tail = 0;
  const int size = 64;  // The buffer allows usage of size - 1 slots so that we
                        // can test for fullness without checking every put/get
  // The final number of usable slots is equivalent to the user's passed size
  // to the constructor size
  // Also cannot be unsigned because it is used for negative modulus calculation
};

// Class implementation

template <typename T>
RingBuffer<T>::RingBuffer(int n) : size{n + 1} {
  if (size <= 0) {
    throw std::length_error(
        "RingBuffer::RingBuffer() Cannot create RingBuffer of size 0 or less.");
  }

  // TODO(Caleb): Change out with custom mem alloc
  buffer = new T[size];
}

template <typename T>
RingBuffer<T>::RingBuffer(std::initializer_list<T> il)
    : size{static_cast<int>(il.size()) + 1} {
  if (size <= 0) {
    throw std::length_error(
        "RingBuffer::RingBuffer() Cannot create RingBuffer of size 0 or less.");
  }

  // TODO(Caleb): Change out with custom mem alloc
  buffer = new T[size];
  for (auto i : il) {
    Put(i);
  }
}

template <typename T>
RingBuffer<T>::RingBuffer(std::initializer_list<T> il, int n) : size{n + 1} {
  if (size <= 0) {
    throw std::length_error(
        "RingBuffer::RingBuffer() Cannot create RingBuffer of size 0 or less.");
  } else if (size < il.size()) {
    throw std::length_error(
        "RingBuffer::RingBuffer() Cannot create RingBuffer of size less than "
        "the passed in list.");
  }

  // TODO(Caleb): Change out with custom mem alloc
  buffer = new T[size];
  for (auto i : il) {
    Put(i);
  }
}

template <typename T>
RingBuffer<T>::RingBuffer(const RingBuffer& rb) : size{rb.GetCapacity()} {
  // TODO(Caleb): Change out with custom mem alloc
  buffer = new T[size];

  copyList = rb.ToList();
  for (auto i : copyList) {
    Put(i);
  }
}

template <typename T>
RingBuffer<T>::RingBuffer(RingBuffer&& rb) : size{rb.GetCapacity()} {
  // TODO(Caleb): Change out with custom mem alloc
  buffer = new T[size];

  while (!rb.IsEmpty()) {
    Put(rb.Get());
  }
}

template <typename T>
T RingBuffer<T>::Get() {
  if (IsEmpty()) {
    throw std::out_of_range("RingBuffer::get() Buffer is empty.");
  }

  int idx = head;
  head = (head + 1) % size;

  return buffer[idx];
}

template <typename T>
void RingBuffer<T>::Put(T o) {
  if (IsFull()) {
    throw std::range_error("RingBuffer::put() Buffer is full.");
  }

  buffer[tail] = o;
  tail = (tail + 1) % size;
}

template <typename T>
T RingBuffer<T>::GetBack() {
  if (IsEmpty()) {
    throw std::out_of_range("RingBuffer::getBack() Buffer is empty.");
  }

  int idx = tail;
  tail = std::abs(static_cast<int>((tail - 1) % size));

  return buffer[tail];
}

template <typename T>
void RingBuffer<T>::PutFront(T o) {
  if (IsFull()) {
    throw std::out_of_range("RingBuffer::putBack() Buffer is full.");
  }

  head = ((head - 1) % size + size) % size;
  buffer[head] = o;
}

template <typename T>
void RingBuffer<T>::Clear() {
  head = tail;
}

template <typename T>
T* RingBuffer<T>::ToList() {
  T* list = new T[GetLength()];
  count = 0;
  idx = head;
  while (idx != tail) {
    list[count] = buffer[idx];
    idx = (idx + 1) % size;
    count++;
  }
  return list;
}

template <typename T>
bool RingBuffer<T>::IsEmpty() const {
  return head == tail;
}

template <typename T>
bool RingBuffer<T>::IsFull() const {
  return (tail + 1) % size == head;
}

template <typename T>
int RingBuffer<T>::GetCapacity() const {
  return size - 1;
}

template <typename T>
int RingBuffer<T>::GetLength() const {
  return abs(head - tail);
}
}  // namespace Isetta
