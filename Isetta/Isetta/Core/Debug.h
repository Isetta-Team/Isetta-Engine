/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <Windows.h>
#include <stdio.h>
#include <cstdint>
#include <fstream>
#include <string>
#include "Core/Color.h"
#include "Core/IModule.h"
#include "Core/Math/Math.h"

namespace Isetta {
class Debug : public IModule {
 public:
  enum Verbosity {
    Off,
    Error,
    Warning,
    Info,
  };

  enum Channel {
    General = (1u << 0),
    Memory = (1u << 1),
    Networking = (1u << 2),
    Graphics = (1u << 3),
    Physics = (1u << 4),
    Gameplay = (1u << 5),
    Sound = (1u << 6),
  };

  static Verbosity gVerbosity;
  static uint32_t gChannelMask;
  static bool gAlwaysFlush;

  static void StartUp();
  static void ShutDown();

  static int PrintF(const std::string format, ...);
  static int PrintF(const Channel channel, const std::string format, ...);
  static int PrintF(const Channel channel, const Verbosity verbosity,
                    const std::string format, ...);

  static void LogInfo(const Channel channel, const std::string format, ...);
  static void LogWarning(const Channel channel, const std::string format, ...);
  static void LogError(const Channel channel, const std::string format, ...);

  // static void DrawLine(Math::Vector3 start, Math::Vector3 end,
  //                     Color color = Color::white, float duration = 0.0f,
  //                     bool depthTest = true);
  // static void DrawRay(Math::Vector3 start, Math::Vector3 dir,
  //                    Color color = Color::white, float duration = 0.0f,
  //                    bool depthTest = true);

 protected:
  static int VDebugPrintF(const bool outputDebug, const std::string format,
                          va_list argList);

 private:
  static std::ofstream fout;
  static bool CheckChannelMask(const Channel channel);
  static bool CheckVerbosity(const Verbosity verbosity);

  static Verbosity defaultVerbosity;
  static Channel defaultChannel;

  static inline const std::string ToString(Channel c) {
    switch (c) {
      case General:
        return "General";
      case Memory:
        return "Memory";
      case Networking:
        return "Networking";
      case Graphics:
        return "Graphics";
      case Physics:
        return "Physics";
      case Gameplay:
        return "Gameplay";
      case Sound:
        return "Sound";
      default:
        return "Unknown";
    }
  }
};
}  // namespace Isetta
