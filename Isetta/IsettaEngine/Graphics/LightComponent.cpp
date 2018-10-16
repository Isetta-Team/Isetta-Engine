/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/LightComponent.h"
#include <utility>
#include "Core/Config/Config.h"
#include "Scene/Transform.h"
#include "Util.h"

namespace Isetta {

RenderModule* LightComponent::renderModule{nullptr};

LightComponent::LightComponent(const std::string& resourceName,
                               std::string lightName)
    : name{std::move(lightName)} {
  ASSERT(renderModule != nullptr);
  renderModule->lightComponents.push_back(this);

  renderResource = LoadResourceFromFile(resourceName);
}

H3DRes LightComponent::LoadResourceFromFile(const std::string& resourceName) {
  H3DRes lightMatRes =
      h3dAddResource(H3DResTypes::Material, resourceName.c_str(), 0);

  RenderModule::LoadResourceFromDisk(
      lightMatRes, Util::StrFormat("LightComponent::LoadResourceFromFile => "
                                   "Cannot load the resource from %s",
                                   resourceName.c_str()));

  return lightMatRes;
}

void LightComponent::OnEnable() {
  if (renderNode == 0) {
    renderNode = h3dAddLightNode(H3DRootNode, name.c_str(), renderResource,
                                 "LIGHTING", "SHADOWMAP");
  } else {
    h3dSetNodeFlags(renderNode, 0, true);
  }
}

void LightComponent::OnDisable() {
  h3dSetNodeFlags(renderNode, H3DNodeFlags::Inactive, true);
}

void LightComponent::UpdateH3DTransform() const {
  Transform::SetH3DNodeTransform(renderNode, GetTransform());
}
}  // namespace Isetta
