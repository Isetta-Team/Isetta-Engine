#include "Core/Memory/ObjectHandle.h"

namespace Isetta {


void HandleEntry::Set(const U32 uniqueID, void* ptr, const bool isEmpty) {
  this->uniqueID = uniqueID;
  this->ptr = ptr;
  this->isEmpty = isEmpty;
}

}  // namespace Isetta
