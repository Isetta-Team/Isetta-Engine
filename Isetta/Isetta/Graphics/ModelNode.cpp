/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/ModelNode.h"
#include <Horde3DUtils.h>
#include "Core/Config/Config.h"

namespace Isetta {
ModelNode::ModelNode(std::string resourceName) {
  H3DRes renderResource = ModelNode::LoadResourceFromFile(resourceName);
  // TODO(Chaojie) Scene graph and hierarchy
  renderNode = h3dAddNodes(H3DRootNode, renderResource);
}
ModelNode::ModelNode(std::string resourceName, const Math::Vector3& position,
                     const Math::Vector3& rotation,
                     const Math::Vector3& scale) {
  H3DRes renderResource = ModelNode::LoadResourceFromFile(resourceName);
  // TODO(Chaojie) Scene graph and hierarchy
  renderNode = h3dAddNodes(H3DRootNode, renderResource);
  SetTransform(position, rotation, scale);
}

H3DRes ModelNode::LoadResourceFromFile(std::string resourceName) {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::SceneGraph, resourceName.c_str(), 0);

  RenderModule::LoadResourceFromDisk(
      renderResource,
      Util::StrFormat(
          "ModelNode::LoadResourceFromFile => Cannot load the resource from %s",
          resourceName.c_str()));

  return renderResource;
}
}  // namespace Isetta
