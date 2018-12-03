/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <list>
#include "Core/Config/CVar.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "Horde3D/Horde3D/Bindings/C++/Horde3D.h"

namespace Isetta {
class ISETTA_API_DECLARE RenderModule {
 public:
  struct RenderConfig {
    CVar<int> hordeLoadTextures{"load_textures", 1};
    CVar<int> hordeTexCompression{"tex_compression", 0};
    CVar<int> hordeMaxAnisotropy{"max_abusotropy", 4};
    CVar<int> hordeShadowmapSize{"shadowmap_size", 2048};
    CVar<int> hordeFastAnimation{"fast_animation", 1};
    CVar<int> hordeSampleCount{"render_sample_count", 0};
    CVar<int> hordeDumpFailedShaders{"dump_failed_shaders", 1};
    CVarString hordePipeline{"render_pipeline_file",
                             "pipelines/forward.pipeline.xml"};
  };

  static void LoadResourceFromDisk(H3DRes resource,
                                   const std::string_view errorMessage);

  /**
   * \brief The array of animation nodes
   */
  std::list<class AnimationComponent*> animationComponents;
  std::list<class MeshComponent*> meshComponents;
  std::list<class LightComponent*> lightComponents;
  std::list<class CameraComponent*> cameraComponents;
  std::list<class ParticleSystemComponent*> particleSystemComponents;

 private:
  RenderModule() = default;
  ~RenderModule() = default;

  void StartUp(GLFWwindow* win);
  void Update(float deltaTime);
  void ShutDown();

  int renderInterface;

  void InitRenderConfig();
  void InitHordeConfig();
  void InitH3D();
  void InitResources();

  // Engine objects
  int curPipeline;

  H3DRes pipelineRes;
  GLFWwindow* winHandle;

  friend class CameraComponent;
  friend class EngineLoop;
  friend class StackAllocator;
};
}  // namespace Isetta
