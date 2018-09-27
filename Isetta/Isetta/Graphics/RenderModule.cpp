/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/RenderModule.h"

#include "Horde3DUtils.h"
#include "Core/Config/Config.h"
#include "Graphics/AnimationNode.h"
#include "Core/FileSystem.h"
#include <filesystem>
#include <exception>

namespace Isetta {
void RenderModule::StartUp(GLFWwindow* win) {
  winHandle = win;
  InitRenderConfig();
  InitH3D();
  InitHordeConfig();
  InitResources();
  AnimationNode::renderModule = this;
}

void RenderModule::Update(float deltaTime) {
  if (!cam) return;
  for (const auto& anim : animationNodes) {
    anim->UpdateAnimation(deltaTime);
  }

  h3dRender(cam);

  h3dFinalizeFrame();
}

void RenderModule::ShutDown() {
  if (winHandle) {
    h3dRelease();
  }
}

void RenderModule::InitRenderConfig() {
  renderInterface = H3DRenderDevice::OpenGL4;
  fov = Config::Instance().renderConfig.fieldOfView.GetVal();
  nearPlane = Config::Instance().renderConfig.nearClippingPlane.GetVal();
  farPlane = Config::Instance().renderConfig.farClippingPlane.GetVal();
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
    h3dutDumpMessages();
    throw std::exception("Render::InitH3D: Unable to initalize Horde3D");
  }
}

void RenderModule::InitResources() {  // 1. Add resources
  pipelineRes = h3dAddResource(H3DResTypes::Pipeline,
                                  Config::Instance().renderConfig.hordePipeline.GetVal().c_str(), 0);

  if (!h3dutLoadResourcesFromDisk(resourcePath.c_str())) {
    h3dutDumpMessages();
    throw std::exception(
        "Render::InitPipeline: Error in loading pipeline resources");
  }

  // Probably later
  cam = h3dAddCameraNode(H3DRootNode, "Camera", pipelineRes);
  h3dSetNodeParamI(cam, H3DCamera::OccCullingI, 1);
  h3dSetNodeTransform(cam, 0, 50, 600, 0, 0, 0, 1, 1, 1);

  ResizeViewport();
}

void RenderModule::ResizeViewport() {
  int width, height;
  if (winHandle) {
    glfwGetWindowSize(winHandle, &width, &height);
  } else {
    width = -1;
    height = -1;
  }

  // Resize viewport
  h3dSetNodeParamI(cam, H3DCamera::ViewportXI, 0);
  h3dSetNodeParamI(cam, H3DCamera::ViewportYI, 0);
  h3dSetNodeParamI(cam, H3DCamera::ViewportWidthI, width);
  h3dSetNodeParamI(cam, H3DCamera::ViewportHeightI, height);

  // Set virtual camera parameters
  h3dSetupCameraView(cam, fov, static_cast<float>(width) / height, nearPlane,
                     farPlane);
  h3dResizePipelineBuffers(pipelineRes, width, height);
}

void RenderModule::LoadResourceAsync(H3DRes resource, const char* databuf, int size, std::string errorMessage) {
  // TODO(Chaojie): integrate
  //auto delim = std::filesystem::path::preferred_separator;
  //std::string 
  //FileSystem::Instance().ReadAsync()
}
}  // namespace Isetta
