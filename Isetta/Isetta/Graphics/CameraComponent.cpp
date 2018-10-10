/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/CameraComponent.h"
#include "Core/Debug/Assert.h"
#include "Graphics/RenderModule.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Util.h"

namespace Isetta {
RenderModule* CameraComponent::renderModule{nullptr};
CameraComponent::CameraComponent(std::string cameraName)
    : renderNode(0), renderResource(0), name{cameraName} {
  ASSERT(renderModule != nullptr);
  renderModule->cameraComponents.push_back(this);
}

void CameraComponent::OnEnable() {
  ASSERT(renderModule != nullptr);
  renderNode =
      h3dAddCameraNode(H3DRootNode, name.c_str(), renderModule->pipelineRes);
  h3dSetNodeParamI(renderNode, H3DCamera::OccCullingI, 1);
  ResizeViewport();
}

void CameraComponent::OnDisable() { h3dRemoveNode(renderNode); }
void CameraComponent::UpdateTransform() {
  Transform transform = owner->GetTransform();
  Transform::SetH3DNodeTransform(renderNode, transform);
}

void CameraComponent::ResizeViewport() {
  int width, height;
  ASSERT(renderModule != nullptr);
  ASSERT(renderModule->winHandle != nullptr);
  glfwGetWindowSize(renderModule->winHandle, &width, &height);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportXI, 0);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportYI, 0);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportWidthI, width);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportHeightI, height);
  h3dResizePipelineBuffers(renderModule->pipelineRes, width, height);
}

void CameraComponent::SetupCameraViewport() {
  int width, height;
  ASSERT(renderModule != nullptr);
  ASSERT(renderModule->winHandle != nullptr);
  glfwGetWindowSize(renderModule->winHandle, &width, &height);
  h3dSetupCameraView(renderNode, fov, static_cast<float>(width) / height,
                     nearPlane, farPlane);
}

void CameraComponent::SetCameraProperties(CameraProperties attr, float value) {
  switch (attr) {
    case CameraProperties::FIELD_OF_VIEW:
      fov = value;
      break;
    case CameraProperties::FAR_CLIPPING_PLANE:
      farPlane = value;
      break;
    case CameraProperties::NEAR_CLIPPING_PLANE:
      nearPlane = value;
      break;
  }
  SetupCameraViewport();
}

}  // namespace Isetta
