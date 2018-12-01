/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <string>
#include "Core/Color.h"
#include "Core/Config/CVar.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"
#include "Scene/Component.h"

namespace Isetta {
DEFINE_COMPONENT(LightComponent, Component, false)
public:
struct LightConfig {
  CVar<float> radius{"light_radius", 2500};
  CVar<float> fieldOfView{"light_fov", 180};
  CVar<float> colorMultiplier{"color_multiplier", 1.0f};
  CVarVector3 color{"light_color"};
  CVar<int> shadowMapCount{"shadow_map_count", 1};
  CVar<float> shadowMapBias{"shadow_map_bias", 0.01f};
};

enum class Property {
  RADIUS,
  FOV,
  SHADOW_MAP_COUNT,
  SHADOW_MAP_BIAS,
  COLOR,
  COLOR_MULTIPLIER
};

void Awake() override;
void OnEnable() override;
void OnDisable() override;
void OnDestroy() override;

LightComponent();
explicit LightComponent(std::string_view lightMaterial);

template <Property Attr, typename T>
void SetProperty(T value);

void SetFloatProperty(H3DNode renderNode, int lightProp, int channel, float value);
void SetIntProperty(H3DNode renderNode, int lightProp, int value);

template <Property Attr, typename T>
T GetProperty() const;

private:
static H3DRes LoadResourceFromFile(std::string_view resourceName);
void UpdateH3DTransform() const;

static class RenderModule* renderModule;
friend class RenderModule;
std::string_view name;
H3DNode renderNode{0};
H3DRes renderResource{0};
DEFINE_COMPONENT_END(LightComponent, Component)

template <LightComponent::Property Attr, typename T>
void LightComponent::SetProperty(T value) {
  if constexpr (Attr == Property::RADIUS) {
    SetFloatProperty(renderNode, H3DLight::RadiusF, 0, value);
  } else if constexpr (Attr == Property::FOV) {
    SetFloatProperty(renderNode, H3DLight::FovF, 0, value);
  } else if constexpr (Attr == Property::SHADOW_MAP_COUNT) {
    SetIntProperty(renderNode, H3DLight::ShadowMapCountI, value);
  } else if constexpr (Attr == Property::SHADOW_MAP_BIAS) {
    SetFloatProperty(renderNode, H3DLight::ShadowMapBiasF, 0, value);
  } else if constexpr (Attr == Property::COLOR) {
    SetFloatProperty(renderNode, H3DLight::ColorF3, 0, value.r);
    SetFloatProperty(renderNode, H3DLight::ColorF3, 1, value.g);
    SetFloatProperty(renderNode, H3DLight::ColorF3, 2, value.b);
  } else if constexpr (Attr == Property::COLOR_MULTIPLIER) {
    SetFloatProperty(renderNode, H3DLight::ColorMultiplierF, 0, value);
  }
}

template <LightComponent::Property Attr, typename T>
T LightComponent::GetProperty() const {
  if constexpr (Attr == Property::RADIUS) {
    return h3dGetNodeParamF(renderNode, H3DLight::RadiusF, 0);
  } else if constexpr (Attr == Property::FOV) {
    return h3dGetNodeParamF(renderNode, H3DLight::FovF, 0);
  } else if constexpr (Attr == Property::SHADOW_MAP_COUNT) {
    return h3dGetNodeParamI(renderNode, H3DLight::ShadowMapCountI);
  } else if constexpr (Attr == Property::SHADOW_MAP_BIAS) {
    return h3dGetNodeParamF(renderNode, H3DLight::ShadowMapBiasF, 0);
  } else if constexpr (Attr == Property::COLOR) {
    Color c;
    c.r = h3dGetNodeParamF(renderNode, H3DLight::ColorF3, 0);
    c.g = h3dGetNodeParamF(renderNode, H3DLight::ColorF3, 1);
    c.b = h3dGetNodeParamF(renderNode, H3DLight::ColorF3, 2);
    return c;
  } else if constexpr (Attr == Property::COLOR_MULTIPLIER) {
    return h3dGetNodeParamF(renderNode, H3DLight::ColorMultiplierF, 0);
  }
}
}  // namespace Isetta
