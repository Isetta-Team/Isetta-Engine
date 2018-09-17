/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Windows.h>
#include <string>
#include "Core/Config/CVar.h"
#include "Core/Config/CVarRegistry.h"
#include "Core/Math/Vector3.h"
#include "Graphics/Window.h"
#include "EngineLoop.h"
#include "Graphics/RenderModule.h"

namespace Isetta {
class Config {
 public:
  static Config& Instance() {
    static Config instance;
    return instance;
  }

  WindowModule::WindowConfig windowConfig;
  EngineLoop::LoopConfig loopConfig;
  RenderModule::RenderConfig renderConfig;

  CVarInt integerVar{"integer", 0};
  CVarFloat floatVar{"float"};
  CVarString stringVar{"string", "test"};
  CVarVector3 vector3Var{"vec3", Math::Vector3::one};
  CVarInt defaultValue{"default", 10};
  CVarString resourcePath{"resource_path", ""};

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
