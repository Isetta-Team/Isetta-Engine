/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/ModelNode.h"

namespace Isetta {
ModelNode::ModelNode(std::string resourceName) {
  LoadResourceFromFile(resourceName);
}
ModelNode::ModelNode(std::string resourceName, const Math::Vector3& position,
                     const Math::Vector3& rotation,
                     const Math::Vector3& scale) {
  LoadResourceFromFile(resourceName);
  SetTransform(position, rotation, scale);
}
void ModelNode::LoadResourceFromFile(std::string resourceName) {
  renderResource =
      h3dAddResource(H3DResTypes::SceneGraph, resourceName.c_str(), 0);
  // #TODO(Chaojie) read the resource path from game config
  if (!h3dutLoadResourcesFromDisk(R"(Resources)")) {
    h3dutDumpMessages();
    throw std::exception(
        std::string(
            "ModelNode::LoadResourceFromFile: Cannot load the resource from " +
            resourceName)
            .c_str());
  }
  // #TODO(Chaojie) Scene graph and hierarchy
  renderNode = h3dAddNodes(H3DRootNode, renderResource);
}
}  // namespace Isetta
