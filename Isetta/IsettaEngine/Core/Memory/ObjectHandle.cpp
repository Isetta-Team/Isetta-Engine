/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/ObjectHandle.h"

namespace Isetta {

PtrInt HandleEntry::GetAddress() const { return reinterpret_cast<PtrInt>(ptr); }

void HandleEntry::Set(const U32 uniqueID, void* ptr, const bool isEmpty,
                      const Size size) {
  this->uniqueID = uniqueID;
  this->ptr = ptr;
  this->isEmpty = isEmpty;
  this->size = size;
}
}  // namespace Isetta
