/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <GLFW/glfw3.h>
#include <Horde3D.h>
#include <string>
#include "Core/Config/CVar.h"

namespace Isetta {
class RenderModule {
 public:
  struct RenderConfig {
    CVar<int> hordeLoadTextures{"horde_load_textures", 1};
    CVar<int> hordeTexCompression{"horde_tex_compression", 0};
    CVar<int> hordeMaxAnisotropy{"horde_max_abusotropy", 4};
    CVar<int> hordeShadowmapSize{"horde_shadowmap_size", 2048};
    CVar<int> hordeFastAnimation{"horde_fast_animation", 1};
    CVar<int> hordeSampleCount{"horde_sample_count", 0};
    CVar<int> hordeDumpFailedShaders{"horde_dump_failed_shaders", 1};
    CVar<float> fieldOfView{"field_of_view", 45.0};
    CVar<float> nearClippingPlane{"near_clipping_plane", 0.1};
    CVar<float> farClippingPlane{"far_clipping_plane", 1000.0};
  };

 private:
  RenderModule() = default;
  ~RenderModule() = default;

  void StartUp(GLFWwindow* win);
  void Update(float deltaTime);
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
