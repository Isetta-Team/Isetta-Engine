/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Graphics/RenderNode.h"

namespace Isetta {
class ModelNode : RenderNode {
 public:
  explicit ModelNode(std::string resourceName);
  ModelNode(std::string resourceName, const Math::Vector3& position,
            const Math::Vector3& rotation, const Math::Vector3& scale);

 protected:
  H3DRes LoadResourceFromFile(std::string resourceName) override;

  friend class AnimationNode;
};
}  // namespace Isetta
