/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/MeshComponent.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Assert.h"
#include "Input/Input.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Util.h"

namespace Isetta {
RenderModule* MeshComponent::renderModule{nullptr};

MeshComponent::MeshComponent(std::string resourceName) : Component() {
  // SetAttribute(ComponentAttributes::NEED_UPDATE, false);
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.push_back(this);
  renderResource = LoadResourceFromFile(resourceName);
}

MeshComponent::~MeshComponent() {
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.remove(this);
}

void MeshComponent::UpdateTransform() {
  Transform::SetH3DNodeTransform(renderNode, GetTransform());
}

H3DRes MeshComponent::LoadResourceFromFile(std::string resourceName) const {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::SceneGraph, resourceName.c_str(), 0);

  RenderModule::LoadResourceFromDisk(
      renderResource, Util::StrFormat("MeshComponent::LoadResourceFromFile => "
                                      "Cannot load the resource from %s",
                                      resourceName.c_str()));

  return renderResource;
}

void MeshComponent::OnEnable() {
  renderNode = h3dAddNodes(H3DRootNode, renderResource);
}

void MeshComponent::OnDisable() {
  // Disabling a mesh is also disabling its animation, need to fix it later
  h3dRemoveNode(renderNode);
}

}  // namespace Isetta
