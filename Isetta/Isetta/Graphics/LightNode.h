/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Graphics/RenderNode.h"

namespace Isetta {
class LightNode : RenderNode {
 public:
  explicit LightNode(std::string resourceName);
  LightNode(std::string resourceName, const Math::Vector3& position,
            const Math::Vector3& rotation, const Math::Vector3& scale);

 protected:
  void LoadResourceFromFile(std::string resourceName) override;
};
}  // namespace Isetta
