/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>

namespace Isetta::Debug {
struct Verbosity {
  enum Enum {
    // Off = 0, // enum not needed, can set mask to 0
    Error = (1u << 1),
    Development = (1 << 2),
    Warning = (1u << 3),
    Info = (1u << 4),
    // All = ~0
  };
};

static inline const std::string ToString(Verbosity::Enum v) {
  switch (v) {
    // case Verbosity::Off:
    //  return "Off";
    case Verbosity::Error:
      return "Error";
    case Verbosity::Warning:
      return "Warning";
    case Verbosity::Info:
      return "Info";
    default:
      return "Unknown";
  }
}

struct Channel {
  enum Enum {
    General = (1u << 0),
    Memory = (1u << 1),
    Networking = (1u << 2),
    Graphics = (1u << 3),
    Physics = (1u << 4),
    Gameplay = (1u << 5),
    Sound = (1u << 6),
    FileIO = (1u << 7),
  };
};

static inline const std::string ToString(Channel::Enum c) {
  switch (c) {
    case Channel::General:
      return "General";
    case Channel::Memory:
      return "Memory";
    case Channel::Networking:
      return "Networking";
    case Channel::Graphics:
      return "Graphics";
    case Channel::Physics:
      return "Physics";
    case Channel::Gameplay:
      return "Gameplay";
    case Channel::Sound:
      return "Sound";
    case Channel::FileIO:
      return "File I/O";
    default:
      return "Unknown";
  }
}
}  // namespace Isetta::Debug
