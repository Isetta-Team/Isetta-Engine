/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Config/CVar.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Matrix4.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"
#include "Scene/Component.h"

namespace Isetta {
class Ray;
namespace Math {
class Vector3;
class Vector2;
}  // namespace Math

DEFINE_COMPONENT(CameraComponent, Component, false)
public:
struct CameraConfig {
  CVar<float> fieldOfView{"field_of_view", 45.0};
  CVar<float> nearClippingPlane{"near_clipping_plane", 0.1f};
  CVar<float> farClippingPlane{"far_clipping_plane", 1000.0};
};

enum class Property {
  FOV,
  NEAR_PLANE,
  FAR_PLANE,
  PROJECTION,
};

CameraComponent();

void Awake() override;
void OnEnable() override;
void OnDisable() override;
void OnDestroy() override;

template <Property Attr, typename T>
void SetProperty(T value);

template <Property Attr, typename T>
T GetProperty() const;

static inline const CameraComponent* Main() { return _main; }

Math::Matrix4 GetHordeTransform() const {
  const float* transformPtr;
  h3dGetNodeTransMats(renderNode, nullptr, &transformPtr);
  return Math::Matrix4(transformPtr);
}

Ray ScreenPointToRay(const Math::Vector2& position) const;
Math::Vector2 ScreenToViewportPoint(const Math::Vector2& position) const;
Math::Vector2 ViewportToScreenPoint(const Math::Vector2& position) const;
Ray ViewportPointToRay(const Math::Vector2& position) const;
// TODO(all) ScreenToViewportPoint
// TODO(all) ScreenToWorldPoint
// TODO(all) ViewportPointToRay
// TODO(all) ViewportToScreenPoint
// TODO(all) ViewportToWorldPoint
// TODO(all) WorldToScreenPoint
// TODO(all) WorldToViewportPoint

private:
void UpdateH3DTransform() const;
void ResizeViewport(int width, int height);
void SetupCameraViewport() const;

static inline CameraComponent* _main;

static class RenderModule* renderModule;
friend class RenderModule;

float fov{};
float nearPlane{};
float farPlane{};
Math::Matrix4 projMat;

H3DNode renderNode;
H3DRes renderResource;
U64 resizeHandle;
DEFINE_COMPONENT_END(CameraComponent, Component)

template <CameraComponent::Property Attr, typename T>
void CameraComponent::SetProperty(T value) {
  if constexpr (Attr == Property::PROJECTION) {
    projMat = value;
    h3dSetCameraProjMat(renderNode, projMat.data);
  } else if constexpr (Attr == Property::FOV) {
    fov = value;
  } else if constexpr (Attr == Property::FAR_PLANE) {
    farPlane = value;
  } else if constexpr (Attr == Property::NEAR_PLANE) {
    nearPlane = value;
  }
  SetupCameraViewport();
}

template <CameraComponent::Property Attr, typename T>
T CameraComponent::GetProperty() const {
  if constexpr (Attr == Property::FOV) {
    return fov;
  } else if constexpr (Attr == Property::FAR_PLANE) {
    return farPlane;
  } else if constexpr (Attr == Property::NEAR_PLANE) {
    return nearPlane;
  } else if constexpr (Attr == Property::PROJECTION) {
    float projArr[16];
    h3dGetCameraProjMat(renderNode, projArr);
    return Math::Matrix4(projArr);
  } else {
    return {};
  }
}
}  // namespace Isetta
