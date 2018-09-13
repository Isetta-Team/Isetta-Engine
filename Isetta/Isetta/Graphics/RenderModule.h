/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <string>
#include "Core/IModule.h"

namespace Isetta {
class RenderModule {
 private:
  RenderModule() = default;
  ~RenderModule() = default;

  void StartUp(GLFWwindow* win);
  void Update();
  void ShutDown();

  int renderInterface;
  std::string resourcePath;
  float fov;
  float nearPlane;
  float farPlane;

  void InitRenderConfig();
  void InitHordeConfig();
  void InitH3D();
  void InitResources();
  void ResizeViewport();

  // Engine objects
  int curPipeline;
  H3DRes pipelineRes[3];
  H3DRes fontMatRes, panelMatRes, logoMatRes;
  H3DNode cam;

  GLFWwindow* winHandle;

  friend class ModuleManager;
};
}  // namespace Isetta
