/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"

namespace Isetta {
class Transform {
 public:
  Math::Vector3 position;
  Math::Vector3 rotation;
  Math::Vector3 scale;
};
}  // namespace Isetta