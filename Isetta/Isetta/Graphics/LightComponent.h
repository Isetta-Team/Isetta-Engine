/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <array>
#include <string>
#include "Core/Color.h"
#include "Graphics/RenderNode.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"

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

  template <LightProperties Attr, typename... Args>
  void SetLightProperties(Args&&... args);

 private:
  H3DRes LoadResourceFromFile(std::string resourceName);

  static class RenderModule* renderModule;
  friend class RenderModule;
  void UpdateTransform();
  std::string name;
  H3DNode renderNode;
  H3DRes renderResource;
};

template <LightComponent::LightProperties Attr, typename... Args>
void LightComponent::SetLightProperties(Args&&... args) {
  if constexpr (Attr == LightProperties::LIGHT_RADIUS) {
    h3dSetNodeParamF(renderNode, H3DLight::RadiusF,
                     std::forward<Args>(args)...);
  } else if constexpr (Attr == LightProperties::FIELD_OF_VIEW) {
    h3dSetNodeParamF(renderNode, H3DLight::FovF, std::forward<Args>(args)...);
  } else if constexpr (Attr == LightProperties::SHADOW_MAP_COUNT) {
    h3dSetNodeParamI(renderNode, H3DLight::ShadowMapCountI,
                     std::forward<int>(args)...);
  } else if constexpr (Attr == LightProperties::SHADOW_MAP_BIAS) {
    h3dSetNodeParamF(renderNode, H3DLight::ShadowMapBiasF,
                     std::forward<Args>(args)...);
  } else if constexpr (Attr == LightProperties::COLOR) {
    h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 0,
                     std::array<Color, 1>{args...}[0].r);
    h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 1,
                     std::array<Color, 1>{args...}[0].g);
    h3dSetNodeParamF(renderNode, H3DLight::ColorF3, 2,
                     std::array<Color, 1>{args...}[0].b);
  } else if constexpr (Attr == LightProperties::COLOR_MULTIPLIER) {
    h3dSetNodeParamF(renderNode, H3DLight::ColorMultiplierF,
                     std::forward<Args>(args)...);
  }
}
}  // namespace Isetta
