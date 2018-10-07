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

  template <CameraProperties Attr, typename... Args>
  void SetCameraProperties(Args&&... args);

 private:
  void UpdateTransform();
  void ResizeViewport();
  void SetupCameraViewport();

  void SetCameraProperties(CameraProperties attr, float value);

  static class RenderModule* renderModule;
  friend class RenderModule;

  float fov;
  float nearPlane;
  float farPlane;

  std::string name;
  H3DNode renderNode;
  H3DRes renderResource;
};

template <CameraComponent::CameraProperties Attr, typename ... Args>
void CameraComponent::SetCameraProperties(Args&&... args) {
  SetCameraProperties(Attr, std::forward<Args>(args)...);
}
}  // namespace Isetta
