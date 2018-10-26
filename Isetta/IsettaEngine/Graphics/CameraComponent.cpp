/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/CameraComponent.h"
#include <utility>
#include "Core/Debug/Assert.h"
#include "Graphics/RenderModule.h"
#include "Input/Input.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"
#include "Util.h"

#include "Collisions/Ray.h"
#include "Core/Math/Util.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
RenderModule* CameraComponent::renderModule{nullptr};
CameraComponent* CameraComponent::_main{nullptr};

CameraComponent::CameraComponent(std::string cameraName)
    : name{std::move(cameraName)}, renderNode(NULL), renderResource(NULL) {
  ASSERT(renderModule != nullptr);
  renderModule->cameraComponents.push_back(this);
  if (!_main) {
    _main = this;
  }
}

void CameraComponent::OnEnable() {
  ASSERT(renderModule != nullptr);
  renderNode =
      h3dAddCameraNode(H3DRootNode, name.c_str(), renderModule->pipelineRes);
  h3dSetNodeParamI(renderNode, H3DCamera::OccCullingI, 1);
  int width, height;
  glfwGetWindowSize(renderModule->winHandle, &width, &height);
  ResizeViewport(width, height);
  resizeHandle = Input::RegisterWindowSizeCallback(
      std::bind(&CameraComponent::ResizeViewport, this, std::placeholders::_1,
                std::placeholders::_2));
}

void CameraComponent::OnDisable() {
  h3dRemoveNode(renderNode);
  Input::UnegisterWindowSizeCallback(resizeHandle);
}

Ray Isetta::CameraComponent::ScreenPointToRay(
    const Math::Vector2& position) const {
  int width, height;
  glfwGetWindowSize(renderModule->winHandle, &width, &height);
  float aspect = static_cast<float>(width) / height;
  float tan = Math::Util::Tan(0.5f * fov * Math::Util::DEG2RAD);
  Math::Vector2 pt{(2.f * ((position.x + 0.5f) / width) - 1) * tan * aspect,
                   (1.f - 2.f * ((position.y + 0.5f) / height)) * tan};
  Math::Vector3 o{Math::Vector3::zero};
  o = GetTransform()->WorldPosFromLocalPos(o);
  Math::Vector3 dir =
      GetTransform()->WorldPosFromLocalPos(Math::Vector3{pt, -1});
  dir -= o;
  return Ray{o, dir};
}

void CameraComponent::UpdateH3DTransform() const {
  Transform::SetH3DNodeTransform(renderNode, *GetTransform());
}

void CameraComponent::ResizeViewport(int width, int height) {
  ASSERT(renderModule != nullptr);
  ASSERT(renderModule->winHandle != nullptr);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportXI, 0);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportYI, 0);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportWidthI, width);
  h3dSetNodeParamI(renderNode, H3DCamera::ViewportHeightI, height);
  h3dResizePipelineBuffers(renderModule->pipelineRes, width, height);
}

void CameraComponent::SetupCameraViewport() const {
  int width, height;
  ASSERT(renderModule != nullptr);
  ASSERT(renderModule->winHandle != nullptr);
  glfwGetWindowSize(renderModule->winHandle, &width, &height);
  h3dSetupCameraView(renderNode, fov, static_cast<float>(width) / height,
                     nearPlane, farPlane);
}
}  // namespace Isetta
