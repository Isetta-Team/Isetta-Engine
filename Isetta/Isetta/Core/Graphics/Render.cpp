#include "Render.h"

#include <stdexcept>

namespace Isetta {
GLFWwindow* RenderModule::winHandle{nullptr};

void RenderModule::StartUp() {
  glfwInit();
  InitRenderConfig();
  InitWindow();
  InitH3D();
  InitHordeConfig();
  InitResources();
}

void RenderModule::Update() {
  if (!cam) return;

  glfwPollEvents();

  h3dRender(cam);

  h3dFinalizeFrame();
  h3dutDumpMessages();
  glfwSwapBuffers(winHandle);
}

void RenderModule::ShutDown() {
  if (winHandle) {
    h3dRelease();

    glfwDestroyWindow(winHandle);
    winHandle = nullptr;
  }
}

void RenderModule::InitWindow() {
  // Create OpenGL window
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);

  if (renderInterface == H3DRenderDevice::OpenGL4) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }

  if (winFullScreen) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    winHandle = glfwCreateWindow(mode->width, mode->height, winTitle.c_str(),
                                 glfwGetPrimaryMonitor(), NULL);
  } else {
    winHandle = glfwCreateWindow(initWinWidth, initWinHeight, winTitle.c_str(),
                                 NULL, NULL);
  }

  if (winHandle == NULL) {
    // Fake message box
    glfwDestroyWindow(winHandle);

    winHandle = glfwCreateWindow(800, 50,
                                 "Unable to initialize engine - Make sure you "
                                 "have an OpenGL 2.0 compatible graphics card",
                                 NULL, NULL);
    double startTime = glfwGetTime();
    while (glfwGetTime() - startTime < 5.0) { /* Sleep */
    }

    throw new std::exception(
        "Render::InitWindow: Unable to initialize window. Make sure you have "
        "an OpenGL compatible graphics card.");
  }

  glfwSetWindowUserPointer(winHandle, this);
  glfwMakeContextCurrent(winHandle);

  glfwSetInputMode(winHandle, GLFW_CURSOR,
                   winShowCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void RenderModule::InitRenderConfig() {
  // #TODO Read from game config
  winTitle = "Current Game";
  renderInterface = H3DRenderDevice::OpenGL4;
  fov = 45.f;
  nearPlane = 0.1f;
  farPlane = 1000.f;
  initWinWidth = 1024;
  initWinHeight = 576;
  winFullScreen = false;
  winShowCursor = false;
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
  h3dSetupCameraView(cam, fov, (float)width / height, nearPlane, farPlane);
  h3dResizePipelineBuffers(pipelineRes[0], width, height);
  h3dResizePipelineBuffers(pipelineRes[1], width, height);
  h3dResizePipelineBuffers(pipelineRes[2], width, height);
}
}  // namespace Isetta
