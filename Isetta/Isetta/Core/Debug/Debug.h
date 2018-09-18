/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/IsettaAlias.h"

namespace Isetta::Debug {
enum class Verbosity : U8 {
  Off = 0,  // enum not needed, can set mask to 0
  Error = (1u << 0),
  Warning = (1u << 1),
  Info = (1u << 2),
  All = ~0
};

static inline const std::string ToString(Verbosity v) {
  switch (v) {
    case Verbosity::Off:
      return "Off";
    case Verbosity::Error:
      return "Error";
    case Verbosity::Warning:
      return "Warning";
    case Verbosity::Info:
      return "Info";
    case Verbosity::All:
      return "All";
    default:
      return "Unknown";
  }
}

enum class Channel : U16 {
  General = (1u << 0),
  Memory = (1u << 1),
  Networking = (1u << 2),
  Graphics = (1u << 3),
  Horde3D = (1u << 4),
  Physics = (1u << 5),
  Gameplay = (1u << 6),
  Sound = (1u << 7),
  FileIO = (1u << 8),
};

static inline const std::string ToString(Channel c) {
  switch (c) {
    case Channel::General:
      return "General";
    case Channel::Memory:
      return "Memory";
    case Channel::Networking:
      return "Networking";
    case Channel::Graphics:
      return "Graphics";
    case Channel::Horde3D:
      return "Horde3D";
    case Channel::Physics:
      return "Physics";
    case Channel::Gameplay:
      return "Gameplay";
    case Channel::Sound:
      return "Sound";
    case Channel::FileIO:
      return "File I/O";
    default:
      return "Unknown-Channel";
  }
}
}  // namespace Isetta::Debug
