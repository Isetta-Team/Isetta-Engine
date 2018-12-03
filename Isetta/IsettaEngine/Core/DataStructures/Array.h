/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <initializer_list>
#include <limits>
#include <utility>
#include "Core/Debug/Assert.h"
#include "Core/Math/Util.h"
#include "Core/Memory/MemoryManager.h"
#include "ISETTA_API.h"

namespace Isetta {
template <typename T>
class ISETTA_API_DECLARE Array {
 public:
  class iterator {
   public:
    typedef T value_type;
    typedef T &reference;
    typedef T *pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef int difference_type;
    explicit iterator(pointer ptr) : ptr_(ptr) {}
    iterator operator+(int lhs) {
      iterator i = *this;
      i.ptr_ += lhs;
      return i;
    }
    iterator operator-(int lhs) {
      iterator i = *this;
      i.ptr_ -= lhs;
      return i;
    }
    difference_type operator-(iterator lhs) { return ptr_ - lhs.ptr_; }
    iterator operator++() {
      ++ptr_;
      return *this;
    }
    iterator operator++(int) {
      iterator i = *this;
      ++ptr_;
      return i;
    }
    iterator operator--() {
      --ptr_;
      return *this;
    }
    iterator operator--(int) {
      iterator i = *this;
      --ptr_;
      return i;
    }
    reference operator*() { return *ptr_; }
    pointer operator->() { return ptr_; }
    bool operator==(const iterator &rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const iterator &rhs) { return ptr_ != rhs.ptr_; }

   private:
    pointer ptr_;
  };
  class const_iterator {
   public:
    typedef T value_type;
    typedef T &reference;
    typedef T *pointer;
    typedef int difference_type;
    typedef std::forward_iterator_tag iterator_category;
    explicit const_iterator(pointer ptr) : ptr_(ptr) {}
    const_iterator operator+(int lhs) {
      const_iterator i = *this;
      i.ptr_ -= lhs;
      return i;
    }
    const_iterator operator-(int lhs) {
      const_iterator i = *this;
      i.ptr_ -= lhs;
      return i;
    }
    difference_type operator-(const_iterator lhs) { return ptr_ - lhs.ptr_; }
    const_iterator operator++() {
      ++ptr_;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator i = *this;
      ++ptr_;
      return i;
    }
    const_iterator operator--() {
      --ptr_;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator i = *this;
      --ptr_;
      return i;
    }
    const reference operator*() { return *ptr_; }
    const pointer operator->() { return ptr_; }
    bool operator==(const const_iterator &rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const const_iterator &rhs) { return ptr_ != rhs.ptr_; }

   private:
    pointer ptr_;
  };
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef U64 size_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef T value_type;
  typedef T *pointer;
  typedef T &reference;
  typedef const T &const_reference;

 private:
  T *data;
  size_type size, capacity;

  void ReservePow2(int capacity);

 public:
  Array() : size{0}, capacity{0}, data{nullptr} {}
  explicit Array(size_type capacity) : size{0}, capacity{capacity} {
    capacity = Math::Util::Max(1, capacity);
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
  }
  Array(size_type capacity, const value_type &val)
      : size{capacity}, capacity{capacity} {
    capacity = Math::Util::Max(1, capacity);
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
    for (int i = 0; i < size; ++i) data[i] = val;
  }
  Array(const std::initializer_list<T> &list) : size{0}, capacity{list.size()} {
    capacity = Math::Util::Max(1, capacity);
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
    Insert(begin(), list.begin(), list.end());
  }
  Array(iterator beginIter, iterator endIter)
      : size{0}, capacity{static_cast<size_type>(endIter - beginIter)} {
    capacity = Math::Util::Max(1, capacity);
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
    Insert(begin(), beginIter, endIter);
  }
  Array(const T *beginPtr, const T *endPtr)
      : size{static_cast<size_type>(endPtr - beginPtr)},
        capacity{static_cast<size_type>(endPtr - beginPtr)} {
    capacity = Math::Util::Max(1, capacity);
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
    for (int i = 0; i < capacity; ++i) data[i] = *(beginPtr + i);
  }
  ~Array();

  explicit Array(const std::vector<T> &inVector);
  // explicit operator std::vector<T>();

  Array(const Array &inVector)
      : size{inVector.size}, capacity{inVector.capacity} {
    capacity = Math::Util::Max(1, capacity);
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
    for (int i = 0; i < size; ++i) data[i] = inVector[i];
  }
  Array(Array &&inVector) noexcept
      : data{inVector.data}, size{inVector.size}, capacity{inVector.capacity} {
    inVector.size = inVector.capacity = 0;
    inVector.data = nullptr;
  }
  inline Array &operator=(const Array &inVector) {
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList<T>(capacity, data);
    size = inVector.size;
    capacity = inVector.capacity;
    data = MemoryManager::NewArrOnFreeList<T>(capacity);
    for (int i = 0; i < size; ++i) data[i] = inVector[i];
    return *this;
  }
  inline Array &operator=(Array &&inVector) noexcept {
    if (this->data && this->data != inVector.data)
      MemoryManager::DeleteArrOnFreeList<T>(capacity, data);
    capacity = inVector.capacity;
    size = inVector.size;
    data = inVector.data;
    inVector.size = inVector.capacity = 0;
    inVector.data = nullptr;
    return *this;
  }

  iterator begin() { return iterator(data); }
  const_iterator begin() const { return const_iterator(data); }
  const_iterator cbegin() const noexcept { return const_iterator(data); }
  iterator end() { return iterator(data + size); }
  const_iterator end() const { return const_iterator(data + size); }
  const_iterator cend() const noexcept { return const_iterator(data + size); }
  reverse_iterator rbegin() { return reverse_iterator(data); }
  const_reverse_iterator rbegin() const { return reverse_iterator(data); }
  const_reverse_iterator crbegin() const noexcept {
    return reverse_iterator(data);
  }
  reverse_iterator rend() { return reverse_iterator(data + size); }
  const_reverse_iterator rend() const { return reverse_iterator(data + size); }
  const_reverse_iterator crend() const noexcept {
    return reverse_iterator(data + size);
  }

  inline bool operator==(const Array &rhs) const;
  inline bool operator!=(const Array &rhs) const;

  inline size_type Size() const { return size; }
  inline size_type MaxSize() const { return std::numeric_limits<U64>::max{}; }
  inline size_type Capacity() const { return capacity; }
  bool IsEmpty() const { return size == 0; }
  void Resize(int size, value_type val = value_type());
  void Reserve(int capacity);
  void Shrink();

  inline reference operator[](size_type i);
  inline const_reference operator[](size_type i) const;
  inline reference At(size_type i);
  inline const_reference At(size_type i) const;
  inline reference Front();
  inline const_reference Front() const;
  inline reference Back();
  inline const_reference Back() const;
  inline value_type *Data() { return data; }
  inline const value_type *Data() const { return data; }

  inline void Assign(size_type cnt, const value_type &val);
  inline void Assign(iterator begin, iterator end);
  inline void Assign(const T *begin, const T *end);
  inline void Assign(std::initializer_list<T> list);
  inline void PushBack(value_type &&val);
  inline void PushBack(const value_type &val);
  inline void PopBack();
  inline iterator Insert(iterator position, const value_type &val);
  inline iterator Insert(iterator position, size_type cnt,
                         const value_type &val);
  inline iterator Insert(iterator position, iterator begin, iterator end);
  inline iterator Insert(iterator position, const T *begin, const T *end);
  inline iterator Insert(iterator position, std::initializer_list<T> list);
  inline iterator Erase(iterator position);
  inline iterator Erase(iterator begin, iterator end);
  inline void Swap(Array &x);
  inline void Clear();
  template <typename... Args>
  inline iterator Emplace(iterator position, Args &&... args);
  template <typename... Args>
  inline iterator Emplace(const_iterator position, Args &&... args);
  template <typename... Args>
  inline void EmplaceBack(Args &&... args);

  inline void clear() { Clear(); }
};

template <typename T>
inline Array<T>::~Array() {
  if (capacity > 0) MemoryManager::DeleteArrOnFreeList<T>(capacity, data);
  data = nullptr;
  size = capacity = 0;
}

template <typename T>
inline Array<T>::Array(const std::vector<T> &inVector) {
  size = inVector.size();
  capacity = inVector.capacity();
  data = MemoryManager::NewArrOnFreeList<T>(capacity);
  for (int i = 0; i < size; ++i) {
    data[i] = std::move(inVector[i]);
    inVector[i].~T();
  }
}

template <typename T>
inline bool Array<T>::operator==(const Array &rhs) const {
  if (size != rhs.size) return false;
  for (const_iterator lhsIt = begin(), rhsIt = rhs.begin();
       lhsIt != end(), rhsIt != rhs.end(); ++lhsIt, ++rhsIt) {
    if (lhsIt != rhsIt) return false;
  }
  return true;
}

template <typename T>
inline bool operator==(const Array<T> &lhs, const Array<T> &rhs) {
  return lhs.operator==(rhs);
}

template <typename T>
inline bool Array<T>::operator!=(const Array &rhs) const {
  return !(this == rhs);
}

template <typename T>
inline bool operator!=(const Array<T> &lhs, const Array<T> &rhs) {
  return lhs.operator!=(rhs);
}

template <typename T>
inline void Array<T>::Resize(int inSize, value_type val) {
  if (inSize > capacity) ReservePow2(inSize);
  for (int i = inSize; i < size; ++i) data[i].~T();
  for (int i = size; i < inSize; ++i) data[i] = val;
  size = inSize;
}

template <typename T>
inline void Array<T>::ReservePow2(int inCapacity) {
  inCapacity = Math::Util::NextPowerOfTwo(inCapacity);
  if (inCapacity < capacity) return;
  T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
  for (int i = 0; i < size; ++i) {
    tmpData[i] = std::move(data[i]);
  }
  if (capacity > 0) {
    MemoryManager::DeleteArrOnFreeList<T>(capacity, data);
  }
  capacity = inCapacity;
  data = tmpData;
  tmpData = nullptr;
}

template <typename T>
inline void Array<T>::Reserve(int inCapacity) {
  if (inCapacity < capacity) return;
  T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
  for (int i = 0; i < size; ++i) {
    tmpData[i] = std::move(data[i]);
  }
  if (capacity > 0) {
    MemoryManager::DeleteArrOnFreeList<T>(capacity, data);
  }
  capacity = inCapacity;
  data = tmpData;
}
template <typename T>
inline void Array<T>::Shrink() {
  if (size == capacity) return;
  // TODO(YIDI): Use realloc
  T *tmpData = MemoryManager::NewArrOnFreeList<T>(size);
  for (int i = 0; i < size; ++i) {
    tmpData[i] = std::move(data[i]);
  }
  if (capacity > 0) {
    MemoryManager::DeleteArrOnFreeList<T>(capacity, data);
  }
  capacity = size;
  data = tmpData;
}
template <typename T>
inline typename Array<T>::reference Array<T>::operator[](size_type i) {
  ASSERT(i < size);
  return data[i];
}
template <typename T>
inline
    typename Array<T>::const_reference Array<T>::operator[](size_type i) const {
  ASSERT(i < size);
  return data[i];
}
template <typename T>
inline typename Array<T>::reference Array<T>::At(size_type i) {
  if (i >= size)
    throw new std::out_of_range("Vector::At => index must be less than size");
  return data[i];
}
template <typename T>
inline typename Array<T>::const_reference Array<T>::At(size_type i) const {
  if (i >= size)
    throw new std::out_of_range("Vector::At => index must be less than size");
  return data[i];
}
template <typename T>
inline typename Array<T>::reference Array<T>::Front() {
  if (size == 0)
    throw new std::out_of_range("Vector::Front => size must be greater than 0");
  return data[0];
}
template <typename T>
inline typename Array<T>::const_reference Array<T>::Front() const {
  if (size == 0)
    throw new std::out_of_range("Vector::Front => size must be greater than 0");
  return data[0];
}
template <typename T>
inline typename Array<T>::reference Array<T>::Back() {
  if (size == 0)
    throw new std::out_of_range("Vector::Back => size must be greater than 0");
  return data[size - 1];
}
template <typename T>
inline typename Array<T>::const_reference Array<T>::Back() const {
  if (size == 0)
    throw new std::out_of_range("Vector::Back => size must be greater than 0");
  return data[size - 1];
}
template <typename T>
inline void Array<T>::Assign(size_type cnt, const value_type &val) {
  if (cnt > capacity) ReservePow2(cnt);
  for (int i = 0; i < size; ++i) data[i].~T();
  for (int i = 0; i < cnt; ++i) data[i] = val;
  size = cnt;
}
template <typename T>
inline void Array<T>::Assign(iterator beginIter, iterator endIter) {
  if (endIter - beginIter > capacity) ReservePow2(endIter - beginIter);
  size = endIter - beginIter;
  iterator itThis = begin();
  for (iterator it = beginIter; it != endIter; ++it, ++itThis) {
    (*itThis).~T();
    *itThis = *it;
  }
  for (; itThis != iterator(data + capacity); ++itThis) (*itThis).~T();
}
template <typename T>
inline void Array<T>::Assign(const T *beginPtr, const T *endPtr) {
  if (endPtr - beginPtr > capacity) ReservePow2(endPtr - beginPtr);
  size = endPtr - beginPtr;
  iterator itThis = begin();
  for (const T *it = beginPtr; it != endPtr; ++it, ++itThis) {
    (*itThis).~T();
    *itThis = *it;
  }
  for (; itThis != iterator(data + capacity); ++itThis) (*itThis).~T();
}
template <typename T>
inline void Array<T>::Assign(std::initializer_list<T> list) {
  if (list.size() > capacity) ReservePow2(list.size());
  size = list.size();
  iterator itThis = begin();
  for (auto it = list.begin(); it != list.end(); ++it, ++itThis) {
    (*itThis).~T();
    *itThis = *it;
  }
  for (; itThis != end(); ++itThis) (*itThis).~T();
}
template <typename T>
inline void Array<T>::PushBack(value_type &&val) {
  if (size + 1 > capacity) ReservePow2(size + 1);
  data[size++] = std::move(val);
}
template <typename T>
inline void Array<T>::PushBack(const value_type &val) {
  if (size + 1 > capacity) ReservePow2(size + 1);
  data[size++] = val;
}
template <typename T>
inline void Array<T>::PopBack() {
  if (size == 0)
    throw new std::out_of_range(
        "Vector::PopBack => size must be greater than 0");
  data[--size].~T();
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Insert(iterator position,
                                                    const value_type &val) {
  iterator ret{nullptr};
  int newSize = size + 1;
  if (newSize > capacity) {
    int inCapacity = Math::Util::NextPowerOfTwo(newSize);
    T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
    iterator it = iterator(tmpData), itThis = begin();
    iterator end = iterator(tmpData + newSize);
    for (; it != end, itThis != position; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    *it = val;
    ret = it;
    ++it;
    for (; it != end; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList(capacity, data);
    capacity = inCapacity;
    data = tmpData;
  } else {
    for (iterator it = end(); it != position; --it) *it = *(it - 1);
    *position = val;
    ret = position;
  }
  size = newSize;
  return ret;
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Insert(iterator position,
                                                    size_type cnt,
                                                    const value_type &val) {
  iterator ret{nullptr};
  int newSize = size + cnt;
  if (newSize > capacity) {
    int inCapacity = Math::Util::NextPowerOfTwo(newSize);
    T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
    iterator it = iterator(tmpData), itThis = begin();
    iterator end = iterator(tmpData + newSize);
    for (; it != end, itThis != position; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    for (int i = 0; i < cnt; ++i, ++it) *it = val;
    ret = it;
    for (; it != end; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList(capacity, data);
    capacity = inCapacity;
    data = tmpData;
  } else {
    for (iterator it = end(); it != position; --it) *it = *(it - 1);
    for (int i = 0; i < cnt; ++i) *(position + i) = val;
    ret = position + cnt - 1;
  }
  size = newSize;
  return ret;
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Insert(iterator position,
                                                    iterator beginIter,
                                                    iterator endIter) {
  iterator ret{nullptr};
  int newSize = size + (endIter - beginIter);
  if (newSize > capacity) {
    int inCapacity = Math::Util::NextPowerOfTwo(newSize);
    T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
    iterator it = iterator(tmpData), itThis = begin();
    iterator end = iterator(tmpData + newSize);
    for (; it != end, itThis != position; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    for (iterator nit = beginIter; nit != endIter; ++nit, ++it) *it = *nit;
    ret = it;
    for (; it != end; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList(capacity, data);
    capacity = inCapacity;
    data = tmpData;
  } else {
    for (iterator it = end(); it != position; --it) *it = *(it - 1);
    for (iterator nit = beginIter; nit != endIter; ++nit, ++position)
      *position = *nit;
    ret = position;
  }
  size = newSize;
  return ret;
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Insert(iterator position,
                                                    const T *beginPtr,
                                                    const T *endPtr) {
  iterator ret{nullptr};
  int newSize = size + (endPtr - beginPtr);
  if (newSize > capacity) {
    int inCapacity = Math::Util::NextPowerOfTwo(newSize);
    T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
    iterator it = iterator(tmpData), itThis = begin();
    iterator end = iterator(tmpData + newSize);
    for (; it != end, itThis != position; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    for (const T *nit = beginPtr; nit != endPtr; ++nit, ++it) *it = *nit;
    ret = it;
    for (; it != end; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList(capacity, data);
    capacity = inCapacity;
    data = tmpData;
  } else {
    for (iterator it = end(); it != position; --it) *it = *(it - 1);
    for (const T *nit = beginPtr; nit != endPtr; ++nit, ++position)
      *position = *nit;
    ret = position;
  }
  size = newSize;
  return ret;
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Insert(
    iterator position, std::initializer_list<T> list) {
  return Insert(position, list.begin(), list.end());
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Erase(iterator position) {
  iterator ret = position + 1;
  (*position).~T();
  for (iterator it = position; it != end() - 1; ++it) *it = *(it + 1);
  size--;
  return ret;
}
template <typename T>
inline typename Array<T>::iterator Array<T>::Erase(iterator beginIter,
                                                   iterator endIter) {
  for (iterator it = beginIter; it != endIter; ++it) (*it).~T();
  iterator itEnd = endIter;
  for (iterator itBeg = beginIter; itEnd != end(); ++itBeg, ++itEnd)
    *itBeg = *itEnd;
  size -= (endIter - beginIter);
  return itEnd;
}
template <typename T>
inline void Array<T>::Swap(Array &x) {
  std::swap(data, x.data);
  std::swap(size, x.size);
  std::swap(capacity, x.capacity);
}
template <typename T>
inline void Array<T>::Clear() {
  for (int i = 0; i < size; ++i) data[i].~T();
  size = 0;
}
template <typename T>
template <typename... Args>
inline typename Array<T>::iterator Array<T>::Emplace(iterator position,
                                                     Args &&... args) {
  iterator ret{nullptr};
  int newSize = size + 1;
  if (newSize > capacity) {
    int inCapacity = Math::Util::NextPowerOfTwo(newSize);
    T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
    iterator it = iterator(tmpData), itThis = begin();
    iterator end = iterator(tmpData + newSize);
    for (; it != end, itThis != position; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    *it = T(std::forward<Args>(args)...);
    ret = it;
    ++it;
    for (; it != end; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList(capacity, data);
    capacity = inCapacity;
    data = tmpData;
  } else {
    for (iterator it = end(); it != position; --it) *it = *(it - 1);
    *position = T(std::forward<Args>(args)...);
    ret = position;
  }
  size = newSize;
  return ret;
}
template <typename T>
template <typename... Args>
inline typename Array<T>::iterator Array<T>::Emplace(const_iterator position,
                                                     Args &&... args) {
  iterator ret{nullptr};
  int newSize = size + 1;
  if (newSize > capacity) {
    int inCapacity = Math::Util::NextPowerOfTwo(newSize);
    T *tmpData = MemoryManager::NewArrOnFreeList<T>(inCapacity);
    iterator it = iterator(tmpData), itThis = begin();
    iterator end = iterator(tmpData + newSize);
    for (; it != end, itThis != position; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    *it = T(std::forward<Args>(args)...);
    ret = it;
    ++it;
    for (; it != end; ++it, ++itThis) {
      *it = *itThis;
      (*itThis).~T();
    }
    if (capacity > 0) MemoryManager::DeleteArrOnFreeList(capacity, data);
    capacity = inCapacity;
    data = tmpData;
  } else {
    for (iterator it = end(); it != position; --it) *it = *(it - 1);
    *position = T(args);
    ret = position;
  }
  size = newSize;
  return ret;
}
template <typename T>
template <typename... Args>
inline void Array<T>::EmplaceBack(Args &&... args) {
  if (size + 1 > capacity) ReservePow2(size + 1);
  data[size++] = T(std::forward<Args>(args)...);
}
}  // namespace Isetta
