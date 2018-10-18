/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#pragma once
#include "Core/Math/Vector2.h"
#include "ISETTA_API.h"
#include "Scene/Component.h"

namespace Isetta {
class ISETTA_API GridComponent : public Component {
 public:
  void Update() override;
};
}  // namespace Isetta
