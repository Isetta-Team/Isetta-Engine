/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "RenderNode.h"
#include "Scene/Component.h"

namespace Isetta {
class CameraComponent : public Component {
 public:
  enum class CameraProperties {
    FIELD_OF_VIEW,
    NEAR_CLIPPING_PLANE,
    FAR_CLIPPING_PLANE,
  };

  explicit CameraComponent(std::string cameraName);

  void OnEnable() override;
  void OnDisable() override;

  template <CameraProperties Attr, typename T>
  void SetProperties(T value);

  template <CameraProperties Attr, typename T>
  T GetProperties();

 private:
  void UpdateTransform();
  void ResizeViewport();
  void SetupCameraViewport();

  static class RenderModule* renderModule;
  friend class RenderModule;

  float fov;
  float nearPlane;
  float farPlane;

  std::string name;
  H3DNode renderNode;
  H3DRes renderResource;
};

template <CameraComponent::CameraProperties Attr, typename T>
void CameraComponent::SetProperties(T value) {
  if constexpr (Attr == CameraProperties::FIELD_OF_VIEW) {
    fov = value;
  } else if constexpr (Attr == CameraProperties::FAR_CLIPPING_PLANE) {
    farPlane = value;
  } else if constexpr (Attr == CameraProperties::NEAR_CLIPPING_PLANE) {
    nearPlane = value;
  }
  SetupCameraViewport();
}

template <CameraComponent::CameraProperties Attr, typename T>
T CameraComponent::GetProperties() {
  if constexpr (Attr == CameraProperties::FIELD_OF_VIEW) {
    return fov;
  } else if constexpr (Attr == CameraProperties::FAR_CLIPPING_PLANE) {
    return farPlane;
  } else if constexpr (Attr == CameraProperties::NEAR_CLIPPING_PLANE) {
    return nearPlane;
  } else {
    return {};
  }
}
}  // namespace Isetta
