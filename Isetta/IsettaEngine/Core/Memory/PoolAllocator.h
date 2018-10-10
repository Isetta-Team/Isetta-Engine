/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {

class PoolAllocator {
 public:
  /**
   * \brief Construct a pool allocator with `count` slots each of size `chunkSize`
   * \param chunkSize Size of each slot in byte
   * \param count Number of slots
   */
  explicit PoolAllocator(Size chunkSize, Size count);

  /**
   * \brief Default destructor that does nothing.
   */
  ~PoolAllocator() = default;

  /**
   * \brief Get a pointer to an empty slot
   * \return A raw pointer to an empty slot
   */
  void* Get();

  /**
   * \brief Free a slot
   */
  void Free(void*);

  /**
   * \brief Free all slots and return the memory to the memory manager
   */
  void Erase() const;

 private:
  union Node {
    Node* next;
    explicit Node(Node* next);
  };

  Size capacity;
  Size elementSize;
  Node* head;
  void* memHead;
};

}  // namespace Isetta
