/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

//#include <Windows.h>
#include <string>
#include "Audio/AudioModule.h"
#include "Collisions/CollisionsModule.h"
#include "Core/Config/CVar.h"
#include "Core/Config/CVarRegistry.h"
#include "Core/DataStructures/Array.h"
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryManager.h"
#include "EngineLoop.h"
#include "Graphics/CameraComponent.h"
#include "Graphics/GUIModule.h"
#include "Graphics/LightComponent.h"
#include "Graphics/RenderModule.h"
#include "Graphics/WindowModule.h"
#include "Networking/NetworkingModule.h"
#include "Scene/LevelManager.h"

namespace Isetta {
#define CONFIG_VAL(expr) Isetta::Config::Instance().expr.GetVal()
#define CONFIG_M_VAL(mod, expr) Isetta::Config::Instance().mod.expr.GetVal()

/**
 * @brief [Singleton] Handles parsing configuration file
 * and storing all the CVars that are read from the configuration file for the
 * game. CVars not in this class will not be set by the configuration file.
 */
class ISETTA_API Config {
 public:
  /**
   * @brief Singleton class instance
   *
   */
  static Config &Instance() {
    static Config instance;
    return instance;
  }

  Logger::LoggerConfig logger;
  WindowModule::WindowConfig windowConfig;
  EngineLoop::LoopConfig loopConfig;
  RenderModule::RenderConfig renderConfig;
  CameraComponent::CameraConfig cameraConfig;
  LightComponent::LightConfig lightConfig;
  GUIModule::GUIModuleConfig guiConfig;
  NetworkingModule::NetworkConfig networkConfig;
  MemoryManager::MemoryConfig memoryConfig;
  AudioModule::AudioConfig audioConfig;
  LevelManager::LevelConfig levelConfig;
  CollisionsModule::CollisionConfig collisionConfig;
  Debug::DrawConfig drawConfig;

  /// File path for the resources of game/engine
  CVarString resourcePath{"resource_path", "Resources"};

  /**
   * @brief Use the Filesystem to read the file, then call ProcessFile to parse
   * the file and set the configuration variables
   *
   * @param filePath of the configuration file
   */
  void Read(const std::string_view &filePath);
  /**
   * @brief Process the content passed in by removing the comments, ignoring
   * whitespace (keeps string whitespace in values, not keys), check for valid
   * lines, set the CVar values
   *
   * @param contentBuffer what will be processed into the CVars
   */
  void ProcessFile(const char *contentBuffer);

  void SetVal(const std::string &key, const std::string_view &value);
  Array<std::string_view> GetCommands() const;

 private:
  Config() = default;
  class CVarRegistry cvarsRegistry;

  /**
   * @brief Removes comments (any characters followed by '#') and carriage
   * returns ('\r')
   *
   * @param line a mutable string
   */
  void RemoveComments(std::string *line) const;
  /**
   * @brief Checks if the line is only whitespaces or if it has characters,
   * called by ProcessFile
   *
   * @param line should be a single line (only 1 carriage return)
   * @return true if it is only whitespace
   * @return false if it has more than just whitespace
   */
  bool OnlyWhitespace(const std::string_view &line) const;
  /**
   * @brief Checks if the line is valid, doesn't have multiple equal signs
   *
   * @param line should be a single line (only 1 carriage return)
   * @return true if the line is valid
   * @return false if the line has errors
   */
  bool ValidLine(const std::string_view &line) const;
  /**
   * @brief Extracts the key from the string (key is before the '=')
   *
   * @param key passed by reference value of the key
   * @param sepPos position of the '=' in the line
   * @param line the string which holds key/value pair
   */
  void ExtractKey(std::string *key, const Size &sepPos,
                  const std::string_view line);
  /**
   * @brief Extracts the value from the string (value is after the '=')
   *
   * @param value passed by reference value of the value
   * @param sepPos position of the '=' in the line
   * @param line the string which holds key/value pair
   */
  void ExtractValue(std::string *value, const Size &sepPos,
                    const std::string_view line);
};
}  // namespace Isetta
