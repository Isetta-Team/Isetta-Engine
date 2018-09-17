/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <Horde3D.h>
#include <string>

namespace Isetta::Math {
class Vector3;
}

namespace Isetta {
class RenderNode {
 protected:
  std::string resourceFilepath;
  H3DNode renderNode;

  RenderNode() = default;
  virtual H3DRes LoadResourceFromFile(std::string resourceName) = 0;

 public:
  virtual ~RenderNode() = default;
  // TODO(Chaojie) Transform class
  void SetTransform(const Math::Vector3& position,
                    const Math::Vector3& rotation, const Math::Vector3& scale);

};
}  // namespace Isetta
