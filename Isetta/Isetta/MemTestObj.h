#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {
typedef struct MemTestObj {
  MemTestObj() = default;

  U64 num1;
  U64 nums[100];
};
}  // namespace Isetta