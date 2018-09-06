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
class RenderModule : IModule {
 public:
  // #TODO InputModule depends on this, discuss how to design it
  // Maybe Window Manager -> Render Module, Input Module
  static GLFWwindow* winHandle;

 private:
  RenderModule() = default;
  ~RenderModule() final = default;

  void StartUp() final;
  void Update() final;
  void ShutDown() final;

  int renderInterface;
  std::string resourcePath;
  std::string winTitle;
  int initWinWidth;
  int initWinHeight;
  float fov;
  float nearPlane;
  float farPlane;
  bool winFullScreen;
  bool winShowCursor;

  void InitWindow();
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

  friend class ModuleManager;
};
}  // namespace Isetta
