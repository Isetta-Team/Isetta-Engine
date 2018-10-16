/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/MeshComponent.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Assert.h"
#include "Scene/Transform.h"
#include "Util.h"

namespace Isetta {
RenderModule* MeshComponent::renderModule{nullptr};

MeshComponent::MeshComponent(const std::string& resourceName) : Component() {
  // SetAttribute(ComponentAttributes::NEED_UPDATE, false);
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.push_back(this);
  renderResource = LoadResourceFromFile(resourceName);
}

MeshComponent::~MeshComponent() {
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.remove(this);
}

void MeshComponent::UpdateTransform() const {
  Transform::SetH3DNodeTransform(renderNode, GetTransform());
}

H3DRes MeshComponent::LoadResourceFromFile(const std::string& resourceName) {
  H3DRes renderResource =
      h3dAddResource(H3DResTypes::SceneGraph, resourceName.c_str(), 0);

  RenderModule::LoadResourceFromDisk(
      renderResource, Util::StrFormat("MeshComponent::LoadResourceFromFile => "
                                      "Cannot load the resource from %s",
                                      resourceName.c_str()));

  return renderResource;
}

void MeshComponent::OnEnable() {
  if (renderNode == 0) {
    renderNode = h3dAddNodes(H3DRootNode, renderResource);
  } else {
    h3dSetNodeFlags(renderNode, 0, true);
  }
}

void MeshComponent::OnDisable() {
  h3dSetNodeFlags(renderNode, H3DNodeFlags::Inactive, true);
}

}  // namespace Isetta
