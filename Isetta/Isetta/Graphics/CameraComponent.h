/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Matrix4.h"
#include "RenderNode.h"
#include "Scene/Component.h"

namespace Isetta {
class CameraComponent : public Component {
 public:
  enum class Property {
    FOV,
    NEAR_PLANE,
    FAR_PLANE,
    PROJECTION,
  };

  explicit CameraComponent(std::string cameraName);

  void OnEnable() override;
  void OnDisable() override;

  template <Property Attr, typename T>
  void SetProperty(T value);

  template <Property Attr, typename T>
  T GetProperty() const;

  inline static const CameraComponent* Main() { return _main; }

 private:
  void UpdateTransform();
  void ResizeViewport();
  void SetupCameraViewport();

  static CameraComponent* _main;

  static class RenderModule* renderModule;
  friend class RenderModule;

  float fov;
  float nearPlane;
  float farlane;
  Math::Matrix4 projMat;
  bool isDirty;

  std::string name;
  H3DNode renderNode;
  H3DRes renderResource;
};

template <CameraComponent::Property Attr, typename T>
void CameraComponent::SetProperty(T value) {
  isDirty = true;
  if constexpr (Attr == Property::PROJECTION) {
    projMat = value;
    h3dSetCameraProjMat(renderNode, projMat.data);
    return;
  }
  if constexpr (Attr == Property::FOV) {
    fov = value;
  } else if constexpr (Attr == Property::FAR_PLANE) {
    FAR_PLANEPlane = value;
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
    if (isDirty) {
      float projArr[16];
      h3dGetCameraProjMat(renderNode, projArr);
      projMat = Math::Matrix4(projArr);
    }
    return projMat;
  } else {
    return {};
  }
}
}  // namespace Isetta
