/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/MeshComponent.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Assert.h"
#include "Scene/Entity.h"

namespace Isetta {
RenderModule* MeshComponent::renderModule{nullptr};

MeshComponent::MeshComponent() : Component() {
  SetAttribute(ComponentAttributes::NEED_UPDATE, false);
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.push_back(this);
}

MeshComponent::~MeshComponent() {
  ASSERT(renderModule != nullptr);
  renderModule->meshComponents.remove(this);
}

void MeshComponent::UpdateTransform() {
  Transform transform = owner->GetTransform();
  h3dSetNodeTransform(renderNode, transform.position.x, transform.position.y,
                      transform.position.z, transform.rotation.x,
                      transform.rotation.y, transform.rotation.z,
                      transform.scale.x, transform.scale.y, transform.scale.z);
}

void MeshComponent::LoadResource(std::string resourceName) {
  H3DRes renderResource = LoadResourceFromFile(resourceName);
  // TODO(Chaojie) Scene graph and hierarchy
  renderNode = h3dAddNodes(H3DRootNode, renderResource);
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
}  // namespace Isetta
