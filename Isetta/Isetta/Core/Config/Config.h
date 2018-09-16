/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Windows.h>
#include <string>
#include "Core/Config/CVar.h"
#include "Core/Config/CVarRegistry.h"
#include "Core/Math/Vector3.h"

namespace Isetta {
class Config {
 public:
  static Config& Instance() {
    static Config instance;
    return instance;
  }

  CVarInt integerVar{"integer", 0};
  CVarFloat floatVar{"float"};
  CVarString stringVar{"string", "test"};
  CVarVector3 vector3Var{"vec3", Math::Vector3::one};
  CVarInt defaultValue{"default", 10};
  CVarInt fixedInterval = {"fixed_interval_milliseconds", 16};
  CVarInt maxSimCount = {"max_simulation_count", 5};
  CVarString resourcePath{"resource_path", ""};
  CVarInt windowWidth{"window_width", 1024};
  CVarInt windowHeight{"window_height", 768};
  CVarString windowTitle{"window_title", "Game"};
  CVarInt windowFullScreen{"window_fullscreen", 0};
  CVarInt windowShowCursor{"window_show_cursor", 1};
  CVarInt hordeLoadTextures{"horde_load_textures", 1};
  CVarInt hordeTexCompression{"horde_tex_compression", 0};
  CVarInt hordeMaxAnisotropy{"horde_max_abusotropy", 4};
  CVarInt hordeShadowmapSize{"horde_shadowmap_size", 2048};
  CVarInt hordeFastAnimation{"horde_fast_animation", 1};
  CVarInt hordeSampleCount{"horde_sample_count", 0};
  CVarInt hordeDumpFailedShaders{"horde_dump_failed_shaders", 1};
  CVarFloat fieldOfView{"field_of_view", 45.0};
  CVarFloat nearClippingPlane{"near_clipping_plane", 0.1};
  CVarFloat farClippingPlane{"far_clipping_plane", 1000.0};

  void Read(const std::string& filepath);
  void ProcessFile(const char* contentBuffer);

 private:
  Config() = default;
  class CVarRegistry cvarsRegistry;

  void RemoveComments(std::string* line) const;
  bool OnlyWhitespace(const std::string& line) const;
  bool ValidLine(const std::string& line) const;
  void ExtractKey(std::string* key, const size_t& sepPos,
                  const std::string line);
  void ExtractValue(std::string* value, const size_t& sepPos,
                    const std::string line);

  HANDLE readFile;
};
}  // namespace Isetta
