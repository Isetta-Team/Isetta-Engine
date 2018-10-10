/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/RenderModule.h"

#include <exception>
#include <filesystem>
#include "Core/Config/Config.h"
#include "Core/Filesystem.h"
#include "Core/Math/Vector3.h"
#include "Core/Time/StopWatch.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Horde3DUtils.h"
#include "Scene/Entity.h"

namespace Isetta {
// TODO(Chaojie) remove
std::string RenderModule::resourcePath{};

void RenderModule::StartUp(GLFWwindow* win) {
  winHandle = win;
  InitRenderConfig();
  InitH3D();
  InitHordeConfig();
  InitResources();
  AnimationComponent::renderModule = this;
  MeshComponent::renderModule = this;
  LightComponent::renderModule = this;
  CameraComponent::renderModule = this;
}

void RenderModule::Update(float deltaTime) {
  for (const auto& mesh : meshComponents) {
    bool isTransformDirty =
        mesh->owner->GetAttribute(Entity::EntityAttributes::IS_TRANSFORM_DIRTY);
    // TODO(YIDI): Remove this when finish debugging
#if _DEBUG
    isTransformDirty = true;
#endif
    if (isTransformDirty) {
      mesh->UpdateTransform();
    }
  }
  for (const auto& light : lightComponents) {
    if (light->owner->GetAttribute(
            Entity::EntityAttributes::IS_TRANSFORM_DIRTY)) {
      light->UpdateTransform();
    }
  }
  for (const auto& anim : animationComponents) {
    anim->UpdateAnimation(deltaTime);
  }
  for (const auto& cam : cameraComponents) {
    cam->UpdateTransform();
  }
  ASSERT(cameraComponents.size() > 0);
  CameraComponent::_main = cameraComponents.front();
  h3dRender(cameraComponents.front()->renderNode);

  h3dFinalizeFrame();
}

void RenderModule::ShutDown() {
  if (winHandle) {
    h3dRelease();
  }
}

void RenderModule::InitRenderConfig() {
  renderInterface = H3DRenderDevice::OpenGL4;
  resourcePath = Config::Instance().resourcePath.GetVal();
}

void RenderModule::InitHordeConfig() {
  h3dSetOption(H3DOptions::MaxLogLevel, 0);
  h3dSetOption(H3DOptions::LoadTextures,
               Config::Instance().renderConfig.hordeLoadTextures.GetVal());
  h3dSetOption(H3DOptions::TexCompression,
               Config::Instance().renderConfig.hordeTexCompression.GetVal());
  h3dSetOption(H3DOptions::MaxAnisotropy,
               Config::Instance().renderConfig.hordeMaxAnisotropy.GetVal());
  h3dSetOption(H3DOptions::ShadowMapSize,
               Config::Instance().renderConfig.hordeShadowmapSize.GetVal());
  h3dSetOption(H3DOptions::FastAnimation,
               Config::Instance().renderConfig.hordeFastAnimation.GetVal());
  h3dSetOption(H3DOptions::SampleCount,
               Config::Instance().renderConfig.hordeSampleCount.GetVal());
  h3dSetOption(H3DOptions::DumpFailedShaders,
               Config::Instance().renderConfig.hordeDumpFailedShaders.GetVal());
}

void RenderModule::InitH3D() {
  if (!h3dInit(static_cast<H3DRenderDevice::List>(renderInterface))) {
    throw std::exception("Render::InitH3D: Unable to initalize Horde3D");
  }
}

void RenderModule::InitResources() {  // 1. Add resources
  pipelineRes = h3dAddResource(
      H3DResTypes::Pipeline,
      Config::Instance().renderConfig.hordePipeline.GetVal().c_str(), 0);

  LoadResourceFromDisk(
      pipelineRes,
      "Render::InitPipeline => Error in loading pipeline resources");
}

void RenderModule::LoadResourceFromDisk(H3DRes resource,
                                        std::string errorMessage) {
  // horde3d loading won't load all resource files, it only load current
  // resource and ad nested resources into the resource list as unloaded
  // resources. So here, I need to iteratively load all unloaded resources.
  // Assumption: the resource handle is always increasing
  while (resource != 0 && !h3dIsResLoaded(resource)) {
    std::string filepath{h3dGetResName(resource)};
    Filesystem::Concat({resourcePath}, &filepath);
    int fileSize = Filesystem::Instance().GetFileLength(filepath);
    auto data = Filesystem::Instance().Read(filepath.c_str());
    if (!h3dLoadResource(resource, data, fileSize)) {
      throw std::exception{errorMessage.c_str()};
    }

    delete[] data;
    // Use undefined to return all kinds of resources
    resource = h3dGetNextResource(H3DResTypes::Undefined, resource);
  }
}
}  // namespace Isetta
