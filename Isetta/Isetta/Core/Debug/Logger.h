/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

// TODO put in the Filesystem Module
#define __FILENAME__ \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include <cstdint>
#include <initializer_list>
#include <string>

namespace Isetta {
#define LOG LogObject(__FILENAME__, __LINE__)
#define LOG_INFO LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Info)
#define LOG_WARNING LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Warning)
#define LOG_ERROR LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Error)

namespace Debug {
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
    default:
      return "Unknown";
  }
}
}  // namespace Debug

class Logger {
 public:
  static uint8_t gVerbosityMask;
  static uint32_t gChannelMask;
  static bool gAlwaysFlush, gBreakOnError;

  static void SetLoggerFiles(std::ofstream* inEngineStream,
                             std::ofstream* inChannelStream = nullptr);
  static void SetLoggerChannelFile(std::ofstream* inChannelStream);

  static int DebugPrintF(const std::string file, const int line,
                         const Debug::Channel::Enum channel,
                         const Debug::Verbosity::Enum verbosity,
                         const std::string format, va_list argList);

 protected:
  static int VDebugPrintF(const Debug::Channel::Enum channel,
                          const Debug::Verbosity::Enum verbosity,
                          const std::string format, va_list argList);

 private:
  static std::ofstream* engineStream;
  static std::ofstream* channelStream;
  static bool CheckChannelMask(const Debug::Channel::Enum channel);
  static bool CheckVerbosity(const Debug::Verbosity::Enum verbosity);
};

struct LogObject {
  std::string file;
  int line;
  Debug::Verbosity::Enum verbosity = Debug::Verbosity::Info;

  LogObject(const std::string file, const int line) : file{file}, line{line} {}
  LogObject(const std::string file, const int line,
            Debug::Verbosity::Enum verbosity)
      : file{file}, line{line}, verbosity{verbosity} {}

  void operator()(const Debug::Channel::Enum channel,
                  const Debug::Verbosity::Enum verbosity, const char* inFormat,
                  ...) const;
  void operator()(const Debug::Channel::Enum channel,
                  const Debug::Verbosity::Enum verbosity,
                  const std::string& inFormat) const;
  void operator()(const Debug::Channel::Enum channel,
                  const Debug::Verbosity::Enum verbosity,
                  const std::initializer_list<std::string>& inFormat) const;
  //
  void operator()(const Debug::Channel::Enum channel, const char* inFormat,
                  ...) const;
  void operator()(const Debug::Channel::Enum channel,
                  const std::string& inFormat) const;
  void operator()(const Debug::Channel::Enum channel,
                  const std::initializer_list<std::string>& inFormat) const;
};
}  // namespace Isetta
