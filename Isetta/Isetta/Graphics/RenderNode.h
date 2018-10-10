/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <Horde3D.h>
#include <string>

namespace Isetta::Math {
class Vector3;
class Matrix4;
}  // namespace Isetta::Math

namespace Isetta {
class RenderNode {
 protected:
  std::string resourceFilepath;
  H3DNode renderNode;

  RenderNode() = default;
  /**
   * \brief Load the resource file from disk (different nodes have different
   * ways to load)
   * \param resourceName The filename of the resource
   */
  virtual H3DRes LoadResourceFromFile(std::string resourceName) = 0;

 public:
  virtual ~RenderNode() = default;
  // TODO(Chaojie) Transform class
  /**
   * \brief Set the transform of this node
   * \param position The position of the node
   * \param rotation The rotation in degree of the node
   * \param scale The scale of the node
   */
  void SetHordeTransform(const Math::Vector3& position,
                         const Math::Vector3& rotation,
                         const Math::Vector3& scale);
};
}  // namespace Isetta
