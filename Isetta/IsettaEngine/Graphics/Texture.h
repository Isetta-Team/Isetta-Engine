/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/IsettaAlias.h"

namespace Isetta {
class Texture {
 public:
  static U8* LoadTexture(std::string_view textureName, int* width,
                         int* height);
};
}  // namespace Isetta