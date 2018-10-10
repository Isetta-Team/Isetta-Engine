/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/LightComponent.h"
#include <Horde3DUtils.h>
#include "Core/Config/Config.h"
#include "Util.h"
#include "Scene/Transform.h"

namespace Isetta {

RenderModule* LightComponent::renderModule{nullptr};

LightComponent::LightComponent(std::string resourceName, std::string lightName)
    : name{lightName} {
  ASSERT(renderModule != nullptr);
  renderModule->lightComponents.push_back(this);

  renderResource = LoadResourceFromFile(resourceName);
}

H3DRes LightComponent::LoadResourceFromFile(std::string resourceName) {
  H3DRes lightMatRes =
      h3dAddResource(H3DResTypes::Material, resourceName.c_str(), 0);

  RenderModule::LoadResourceFromDisk(
      lightMatRes, Util::StrFormat("LightComponent::LoadResourceFromFile => "
                                   "Cannot load the resource from %s",
                                   resourceName.c_str()));

  return lightMatRes;
}

void LightComponent::OnEnable() {
  renderNode = h3dAddLightNode(H3DRootNode, name.c_str(), renderResource,
                               "LIGHTING", "SHADOWMAP");
}

void LightComponent::OnDisable() { h3dRemoveNode(renderNode); }

void LightComponent::UpdateTransform() {
  Transform transform = owner->GetTransform();
  Transform::SetH3DNodeTransform(renderNode, transform);
}
}  // namespace Isetta
