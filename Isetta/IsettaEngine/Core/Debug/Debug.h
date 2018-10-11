/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"

namespace Isetta::Debug {
/**
 * @brief Verbosity of the logger:
 * Off - no messages will be output (only needed for mask)
 * Error - most severe messages
 * Warning - messages to warn the developer
 * Info - messages to inform developers/users
 * All - all messages will be output (only needed for mask)
 */
ISETTA_API enum class Verbosity : U8 {
  Off = 0,
  Error = (1u << 0),
  Warning = (1u << 1),
  Info = (1u << 2),
  All = ~0
};

/**
 * @brief Converts Verbosity enum to string
 *
 */
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

/**
 * @brief Channel of the logger:
 * General - any type of general message
 * Horde3D - messages stripped from Horde3D (so they can be surpressed)
 * All others are messages associated with the modules
 */
ISETTA_API enum class Channel : U16 {
  General = (1u << 0),
  Memory = (1u << 1),
  Networking = (1u << 2),
  Graphics = (1u << 3),
  Horde3D = (1u << 4),
  Physics = (1u << 5),
  Gameplay = (1u << 6),
  Sound = (1u << 7),
  FileIO = (1u << 8),
  GUI = (1u << 9),
};

/**
 * @brief Converts Channel enum to string
 *
 */
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
    case Channel::GUI:
      return "GUI";
    default:
      return "Unknown-Channel";
  }
}
}  // namespace Isetta::Debug
