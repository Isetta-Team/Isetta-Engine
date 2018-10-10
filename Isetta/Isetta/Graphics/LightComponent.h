/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Graphics/RenderNode.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Core/Color.h"

namespace Isetta {
class LightComponent : public Component {
 public:
  enum class LightProperties {
    LIGHT_RADIUS,
    FIELD_OF_VIEW,
    SHADOW_MAP_COUNT,
    SHADOW_MAP_BIAS,
    COLOR,
    COLOR_MULTIPLIER
  };

  void OnEnable() override;
  void OnDisable() override;

  LightComponent(std::string resourceName, std::string lightName);

  template <LightProperties Attr, typename T>
  void SetProperties(T value);

  template <LightProperties Attr, typename T>
  T GetProperties();

 private:
  H3DRes LoadResourceFromFile(std::string resourceName);

  static class RenderModule* renderModule;
  friend class RenderModule;
  void UpdateTransform();
  std::string name;
  H3DNode renderNode;
  H3DRes renderResource;
};

template <LightComponent::LightProperties Attr, typename T>
void LightComponent::SetProperties(T value) {
  if constexpr (Attr == LightProperties::LIGHT_RADIUS) {
    h3dSetNodeParamF(renderNode, H3DLight::RadiusF, 0, value);
  } else if constexpr (Attr == LightProperties::FIELD_OF_VIEW) {
    h3dSetNodeParamF(renderNode, H3DLight::FovF, 0, value);
  } else if constexpr (Attr == LightProperties::SHADOW_MAP_COUNT) {
    h3dSetNodeParamI(renderNode, H3DLight::ShadowMapCountI, value);
  } else if constexpr (Attr == LightProperties::SHADOW_MAP_BIAS) {
    h3dSetNodeParamF(renderNode, H3DLight::ShadowMapBiasF, 0, value);
  } else if constexpr (Attr == LightProperties::COLOR) {
    h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 0, value.r);
    h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 1, value.g);
    h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 2, value.b);
  } else if constexpr (Attr == LightProperties::COLOR_MULTIPLIER) {
    h3dSetNodeParamF(renderNode, H3DLight::ColorMultiplierF, 0, value);
  }
}

template <LightComponent::LightProperties Attr, typename T>
T LightComponent::GetProperties() {
  if constexpr (Attr == LightProperties::LIGHT_RADIUS) {
    return h3dGetNodeParamF(renderNode, H3DLight::RadiusF, 0);
  } else if constexpr (Attr == LightProperties::FIELD_OF_VIEW) {
    return h3dGetNodeParamF(renderNode, H3DLight::FovF, 0);
  } else if constexpr (Attr == LightProperties::SHADOW_MAP_COUNT) {
    return h3dGetNodeParamI(renderNode, H3DLight::ShadowMapCountI);
  } else if constexpr (Attr == LightProperties::SHADOW_MAP_BIAS) {
    return h3dGetNodeParamF(renderNode, H3DLight::ShadowMapBiasF, 0);
  } else if constexpr (Attr == LightProperties::COLOR) {
    Color c;
    c.r = h3dGetNodeParamF(renderNode, H3DLight::ColorF3, 0);
    c.g = h3dGetNodeParamF(renderNode, H3DLight::ColorF3, 1);
    c.b = h3dGetNodeParamF(renderNode, H3DLight::ColorF3, 2);
    return c;
  } else if constexpr (Attr == LightProperties::COLOR_MULTIPLIER) {
    return h3dGetNodeParamF(renderNode, H3DLight::ColorMultiplierF, 0);
  }
}
}  // namespace Isetta
