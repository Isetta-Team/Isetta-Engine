/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Vector3.h"

namespace Isetta {
struct Plane {
  Math::Vector3 position, direction;
};
struct Ray {
  Math::Vector3 position, direction;
};
}  // namespace Isetta
