/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/RenderModule.h"

#include <exception>
#include <filesystem>
#include <string>
#include "Core/Config/Config.h"
#include "Core/Filesystem.h"
#include "Graphics/AnimationComponent.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/LightComponent.h"
#include "Graphics/ParticleSystemComponent.h"
#include "Scene/Entity.h"
#include "brofiler/ProfilerCore/Brofiler.h"

namespace Isetta {
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
  ParticleSystemComponent::renderModule = this;
}

void RenderModule::Update(float deltaTime) {
  BROFILER_CATEGORY("Render Update", Profiler::Color::OliveDrab);

  for (const auto& mesh : meshComponents) {
    bool isTransformDirty = mesh->entity->GetAttribute(
        Entity::EntityAttributes::IS_TRANSFORM_DIRTY);
    if (isTransformDirty) {
      mesh->UpdateTransform();
    }
  }
  for (const auto& light : lightComponents) {
    if (light->entity->GetAttribute(
            Entity::EntityAttributes::IS_TRANSFORM_DIRTY)) {
      light->UpdateH3DTransform();
    }
  }
  for (const auto& anim : animationComponents) {
    anim->UpdateAnimation(deltaTime);
  }
  for (const auto& cam : cameraComponents) {
    cam->UpdateH3DTransform();
  }
  for (const auto& particle : particleSystemComponents) {
    if (particle->entity->GetAttribute(
            Entity::EntityAttributes::IS_TRANSFORM_DIRTY)) {
      particle->UpdateTransform();
    }
    if (particle->hasStarted) {
      particle->UpdateEmitter(deltaTime);
    }
  }
  ASSERT(!cameraComponents.empty());
  CameraComponent::_main = cameraComponents.front();
  h3dRender(CameraComponent::_main->renderNode);

  h3dFinalizeFrame();
}

void RenderModule::ShutDown() {
  if (winHandle) {
    h3dRelease();
  }
}

void RenderModule::InitRenderConfig() {
  renderInterface = H3DRenderDevice::OpenGL4;
}

void RenderModule::InitHordeConfig() {
  h3dSetOption(H3DOptions::MaxLogLevel, 0);
  h3dSetOption(H3DOptions::LoadTextures,
               CONFIG_VAL(renderConfig.hordeLoadTextures));
  h3dSetOption(H3DOptions::TexCompression,
               CONFIG_VAL(renderConfig.hordeTexCompression));
  h3dSetOption(H3DOptions::MaxAnisotropy,
               CONFIG_VAL(renderConfig.hordeMaxAnisotropy));
  h3dSetOption(H3DOptions::ShadowMapSize,
               CONFIG_VAL(renderConfig.hordeShadowmapSize));
  h3dSetOption(H3DOptions::FastAnimation,
               CONFIG_VAL(renderConfig.hordeFastAnimation));
  h3dSetOption(H3DOptions::SampleCount,
               CONFIG_VAL(renderConfig.hordeSampleCount));
  h3dSetOption(H3DOptions::DumpFailedShaders,
               CONFIG_VAL(renderConfig.hordeDumpFailedShaders));
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
                                        const std::string_view errorMessage) {
  PROFILE
  // horde3d loading won't load all resource files, it only load current
  // resource and ad nested resources into the resource list as unloaded
  // resources. So here, I need to iteratively load all unloaded resources.
  // Assumption: the resource handle is always increasing
  const std::string path = CONFIG_VAL(resourcePath);
  while (resource != 0 && !h3dIsResLoaded(resource)) {
    std::string filepath{h3dGetResName(resource)};
    Filesystem::Concat({path}, &filepath);
    int fileSize = Filesystem::Instance().GetFileLength(filepath);
    auto data = Filesystem::Instance().Read(filepath.c_str());
    if (!h3dLoadResource(resource, data, fileSize)) {
      throw std::exception{errorMessage.data()};
    }

    delete[] data;
    // Use undefined to return all kinds of resources
    resource = h3dGetNextResource(H3DResTypes::Undefined, resource);
  }
}
}  // namespace Isetta
