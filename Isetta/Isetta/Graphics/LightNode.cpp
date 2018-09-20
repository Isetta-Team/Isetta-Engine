/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/LightNode.h"
#include <Horde3DUtils.h>
#include "Core/Config/Config.h"

namespace Isetta {
LightNode::LightNode(std::string resourceName) {
  H3DRes lightMatRes = LightNode::LoadResourceFromFile(resourceName);
  renderNode = h3dAddLightNode(H3DRootNode, "Light1", lightMatRes, "LIGHTING",
                               "SHADOWMAP");
  SetupLightProperties();
}

LightNode::LightNode(std::string resourceName, const Math::Vector3& position,
                     const Math::Vector3& rotation,
                     const Math::Vector3& scale) {
  H3DRes lightMatRes = LightNode::LoadResourceFromFile(resourceName);
  renderNode = h3dAddLightNode(H3DRootNode, "Light1", lightMatRes, "LIGHTING",
                               "SHADOWMAP");
  SetupLightProperties();
  SetTransform(position, rotation, scale);
}

H3DRes LightNode::LoadResourceFromFile(std::string resourceName) {
  H3DRes lightMatRes =
      h3dAddResource(H3DResTypes::Material, "materials/light.material.xml", 0);
  // TODO(Chaojie) read the resource path from game config
  if (!h3dutLoadResourcesFromDisk(
          Config::Instance().resourcePath.GetVal().c_str())) {
    h3dutDumpMessages();
    throw std::exception(
        std::string(
            "LightNode::LoadResourceFromFile: Cannot load the resource from " +
            resourceName)
            .c_str());
  }
  return lightMatRes;
}

void LightNode::SetupLightProperties() {
  // TODO(Chaojie) read from the game config
  h3dSetNodeParamF(renderNode, H3DLight::RadiusF, 0, 2500);
  h3dSetNodeParamF(renderNode, H3DLight::FovF, 0, 90);
  h3dSetNodeParamI(renderNode, H3DLight::ShadowMapCountI, 1);
  h3dSetNodeParamF(renderNode, H3DLight::ShadowMapBiasF, 0, 0.01f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 0, 1.0f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 1, 1.0f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 2, 1.0f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorMultiplierF, 0, 1.0f);
}
}  // namespace Isetta
