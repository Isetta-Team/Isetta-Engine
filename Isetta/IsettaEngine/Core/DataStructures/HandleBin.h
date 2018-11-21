/*
 * Copyright (c) 2018 Isetta
 */

#pragma once

#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"

#include <set>

namespace Isetta {

class ISETTA_API HandleBin {
 private:
  std::set<U64> handles;
  U64 topHandle;
  U64 maxHandle;

 public:
  HandleBin();
  HandleBin(U64 start);
  HandleBin(U64 start, U64 max);

  U64 GetHandle();
  void ReturnHandle(U64 handle);
  bool RemoveHandle(U64 handle);
  void Clear();
};

}  // namespace Isetta
