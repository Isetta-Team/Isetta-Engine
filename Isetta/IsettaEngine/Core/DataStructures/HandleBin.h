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
  std::set<int> handles;
  U64 topHandle = 0;
  U64 maxHandle;

 public:
  HandleBin();
  HandleBin(int max);

  int GetHandle();
  void ReturnHandle(int handle);
};

}  // namespace Isetta
