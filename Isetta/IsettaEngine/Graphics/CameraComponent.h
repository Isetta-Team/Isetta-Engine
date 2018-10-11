/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Math/Matrix4.h"
#include "Horde3D.h"
#include "Scene/Component.h"

namespace Isetta {
ISETTA_API class CameraComponent : public Component {
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

  Math::Matrix4 GetHordeTransform() const {
    const float* transformPtr;
    h3dGetNodeTransMats(renderNode, NULL, &transformPtr);
    return Math::Matrix4(transformPtr);
  }

 private:
  void UpdateTransform();
  void ResizeViewport();
  void SetupCameraViewport();

  static CameraComponent* _main;

  static class RenderModule* renderModule;
  friend class RenderModule;

  float fov;
  float nearPlane;
  float farPlane;
  Math::Matrix4 projMat;

  std::string name;
  H3DNode renderNode;
  H3DRes renderResource;
};

template <CameraComponent::Property Attr, typename T>
void CameraComponent::SetProperty(T value) {
  if constexpr (Attr == Property::PROJECTION) {
    projMat = value;
    h3dSetCameraProjMat(renderNode, projMat.data);
    return;
  }
  if constexpr (Attr == Property::FOV) {
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
