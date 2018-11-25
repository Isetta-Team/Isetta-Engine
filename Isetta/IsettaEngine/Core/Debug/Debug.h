/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <string>
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"

namespace Isetta::Debug {
struct DrawConfig {
  CVar<int> bvtDrawAABBs{"bvh_draw_aabbs", 0};
};

/**
 * @brief Verbosity of the logger:
 * Off - no messages will be output (only needed for mask)
 * Error - most severe messages
 * Warning - messages to warn the developer
 * Info - messages to inform developers/users
 * All - all messages will be output (only needed for mask)
 */
ISETTA_API enum class Verbosity : U8 { Off, Error, Warning, Info, All };

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
  General,
  Memory,
  Networking,
  Graphics,
  Horde3D,
  Collisions,
  Gameplay,
  Sound,
  FileIO,
  GUI,
  All
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
    case Channel::Collisions:
      return "Collisions";
    case Channel::Gameplay:
      return "Gameplay";
    case Channel::Sound:
      return "Sound";
    case Channel::FileIO:
      return "File I/O";
    case Channel::GUI:
      return "GUI";
    case Channel::All:
      return "All";
    default:
      return "Unknown-Channel";
  }
}
}  // namespace Isetta::Debug
