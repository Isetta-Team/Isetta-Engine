/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Graphics/RenderNode.h"

namespace Isetta {
class ModelNode : RenderNode {
 public:
  ModelNode(std::string resourceName);
  ModelNode(std::string resourceName, const Math::Vector3& position,
            const Math::Vector3& rotation, const Math::Vector3& scale);

 protected:
  void LoadResourceFromFile(std::string resourceName) override;

 private:
};
}  // namespace Isetta