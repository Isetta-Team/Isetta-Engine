/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Graphics/LightNode.h"

namespace Isetta {
LightNode::LightNode(std::string resourceName) {
  LoadResourceFromFile(resourceName);
}

LightNode::LightNode(std::string resourceName, const Math::Vector3& position,
                     const Math::Vector3& rotation,
                     const Math::Vector3& scale) {
  LoadResourceFromFile(resourceName);
  SetTransform(position, rotation, scale);
}

void LightNode::LoadResourceFromFile(std::string resourceName) {
  H3DRes lightMatRes =
      h3dAddResource(H3DResTypes::Material, "materials/light.material.xml", 0);
  // #TODO(Chaojie) read the resource path from game config
  if (!h3dutLoadResourcesFromDisk(R"(Resources)")) {
    h3dutDumpMessages();
    throw std::exception(
        std::string(
            "LightNode::LoadResourceFromFile: Cannot load the resource from " +
            resourceName)
            .c_str());
  }
  renderNode = h3dAddLightNode(H3DRootNode, "Light1", lightMatRes, "LIGHTING", "SHADOWMAP");
  // #TODO(Chaojie) read from the game config
  h3dSetNodeParamF(renderNode, H3DLight::RadiusF, 0, 2500);
  h3dSetNodeParamF(renderNode, H3DLight::FovF, 0, 90);
  h3dSetNodeParamI(renderNode, H3DLight::ShadowMapCountI, 1);
  h3dSetNodeParamF(renderNode, H3DLight::ShadowMapBiasF, 0, 0.01f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 0, 1.0f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 1, 1.0f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 2, 1.0f);
  h3dSetNodeParamF(renderNode, H3DLight::ColorMultiplierF, 0, 1.0f);
}
}