/*
 * Copyright (c) 2018 Isetta
 */
#include "Graphics/RenderModule.h"

#include <stdexcept>

namespace Isetta {
void RenderModule::StartUp(GLFWwindow* win) {
  winHandle = win;
  InitRenderConfig();
  InitH3D();
  InitHordeConfig();
  InitResources();
}

void RenderModule::Update() {
  if (!cam) return;

  h3dRender(cam);

  h3dFinalizeFrame();
}

void RenderModule::ShutDown() {
  if (winHandle) {
    h3dRelease();
  }
}

void RenderModule::InitRenderConfig() {
  // #TODO(Chaojie) Read from game config
  renderInterface = H3DRenderDevice::OpenGL4;
  fov = 45.f;
  nearPlane = 0.1f;
  farPlane = 1000.f;
  resourcePath = R"(Resources)";
}

void RenderModule::InitHordeConfig() {
  h3dSetOption(H3DOptions::LoadTextures, 1);
  h3dSetOption(H3DOptions::TexCompression, 0);
  h3dSetOption(H3DOptions::MaxAnisotropy, 4);
  h3dSetOption(H3DOptions::ShadowMapSize, 2048);
  h3dSetOption(H3DOptions::FastAnimation, 1);
  h3dSetOption(H3DOptions::SampleCount, 0);
  h3dSetOption(H3DOptions::DumpFailedShaders, 1);
}

void RenderModule::InitH3D() {
  if (!h3dInit((H3DRenderDevice::List)renderInterface)) {
    h3dutDumpMessages();
    throw std::exception("Render::InitH3D: Unable to initalize Horde3D");
  }
}

void RenderModule::InitResources() {  // 1. Add resources
  pipelineRes[0] = h3dAddResource(H3DResTypes::Pipeline,
                                  "pipelines/forward.pipeline.xml", 0);
  pipelineRes[1] = h3dAddResource(H3DResTypes::Pipeline,
                                  "pipelines/deferred.pipeline.xml", 0);
  pipelineRes[2] =
      h3dAddResource(H3DResTypes::Pipeline, "pipelines/hdr.pipeline.xml", 0);

  if (!h3dutLoadResourcesFromDisk(resourcePath.c_str())) {
    h3dutDumpMessages();
    throw std::exception(
        "Render::InitPipeline: Error in loading pipeline resources");
    return;
  }

  // Probably later
  cam = h3dAddCameraNode(H3DRootNode, "Camera", pipelineRes[curPipeline]);
  h3dSetNodeParamI(cam, H3DCamera::OccCullingI, 1);
  h3dSetNodeTransform(cam, 0, 50, 600, 0, 0, 0, 1, 1, 1);

  ResizeViewport();

  h3dutDumpMessages();
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
  h3dResizePipelineBuffers(pipelineRes[0], width, height);
  h3dResizePipelineBuffers(pipelineRes[1], width, height);
  h3dResizePipelineBuffers(pipelineRes[2], width, height);
}
}  // namespace Isetta
