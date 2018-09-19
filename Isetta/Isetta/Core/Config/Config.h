/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Windows.h>
#include <string>
#include "Core/Config/CVar.h"
#include "Core/Config/CVarRegistry.h"
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Math/Vector3.h"
#include "EngineLoop.h"
#include "Graphics/RenderModule.h"
#include "Graphics/Window.h"

namespace Isetta {
/**
 * @brief Test module to demonstrate CVar config struct.
 *
 */
class TestModule {
 public:
  /**
   * @brief Test configuration CVar struct. Demonstrates templated paramters,
   * string, vector, and default value parameter.
   */
  struct TestConfig {
    CVar<int> integerVar{"integer", 10};
    CVar<float> floatVar{"float", 1.12};
    CVarString stringVar{"string", "test"};
    CVarVector3 vector3Var{"vec3", Math::Vector3::one};
    CVar<int> defaultValue{"default"};
  };
};

/**
 * @brief [Singleton] Handles parsing configuration file
 * and storing all the CVars that are read from the configuration file for the
 * game. CVars not in this class will not be set by the configuration file.
 */
class Config {
 public:
  /**
   * @brief Singleton class instance
   *
   */
  static Config &Instance() {
    static Config instance;
    return instance;
  }

  /// TestModule configuration CVars
  TestModule::TestConfig test;
  /// Logger configuartion CVars
  Logger::LoggerConfig logger;
  /// WindowModule configuration CVars
  WindowModule::WindowConfig windowConfig;
  /// EngineLoop configuration CVars
  EngineLoop::LoopConfig loopConfig;
  /// RenderModule configuration CVars
  RenderModule::RenderConfig renderConfig;

  /// Max FPS of the engine
  CVar<int> maxFps = {"max_fps", 16};
  /// Max simulation count of update loop
  CVar<int> maxSimCount = {"max_simulation_count", 5};
  /// File path for the resources of game/engine
  CVarString resourcePath{"resource_path", ""};

  /**
   * @brief Use the FileSystem to read the file, then call ProcessFile to parse the file and
   * set the configuration variables
   * 
   * @param filePath of the configuration file
   */
  void Read(const std::string &filePath);
  /**
   * @brief Process the content passed in by removing the comments, ignoring whitespace (keeps string
   * whitespace in values, not keys), check for valid lines, set the CVar values
   * 
   * @param contentBuffer what will be processed into the CVars
   */
  void ProcessFile(const char *contentBuffer);

 private:
  Config() = default;
  class CVarRegistry cvarsRegistry;

  /**
   * @brief Removes comments (any characters followed by '#') and carriage returns ('\r')
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
  bool OnlyWhitespace(const std::string &line) const;
  /**
   * @brief Checks if the line is valid, doesn't have multiple equal signs
   * 
   * @param line should be a single line (only 1 carriage return)
   * @return true if the line is valid
   * @return false if the line has errors
   */
  bool ValidLine(const std::string &line) const;
  /**
   * @brief Extracts the key from the string (key is before the '=')
   * 
   * @param key passed by reference value of the key
   * @param sepPos position of the '=' in the line
   * @param line the string which holds key/value pair
   */
  void ExtractKey(std::string *key, const Size &sepPos, const std::string line);
  /**
   * @brief Extracts the value from the string (value is after the '=')
   * 
   * @param value passed by reference value of the value
   * @param sepPos position of the '=' in the line
   * @param line the string which holds key/value pair
   */
  void ExtractValue(std::string *value, const Size &sepPos,
                    const std::string line);
};
}  // namespace Isetta
