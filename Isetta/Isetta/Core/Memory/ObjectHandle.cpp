/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

void HandleEntry::Set(const U32 uniqueID, void* ptr, const bool isEmpty, const SizeInt size) {
  this->uniqueID = uniqueID;
  this->ptr = ptr;
  this->isEmpty = isEmpty;
  this->size = size;
}

}  // namespace Isetta
