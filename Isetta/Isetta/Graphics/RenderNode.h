/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <string>

namespace Isetta::Math {
class Vector3;
}

namespace Isetta {
class RenderNode {
 protected:
  H3DRes renderResource;
  std::string resourceFilepath;
  H3DNode renderNode;

  RenderNode() {}
  virtual void LoadResourceFromFile(std::string resourceName) = 0;

 public:
  // TODO(Chaojie) Transform class
  void SetTransform(const Math::Vector3& position,
                    const Math::Vector3& rotation, const Math::Vector3& scale);
};
}  // namespace Isetta
