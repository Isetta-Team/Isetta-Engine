/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

// TODO(jacob) put in the Filesystem Module
#define __FILENAME__ \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include <Windows.h>
#include <cstdint>
#include <initializer_list>
#include <string>
#include "Core/Config/CVar.h"
#include "Core/Debug/Assert.h"
#include "Core/Debug/Debug.h"

namespace Isetta {
#define LOG LogObject(__FILENAME__, __LINE__)
#define LOG_INFO LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Info)
#define LOG_WARNING LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Warning)
#define LOG_ERROR LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Error)

class Logger {
 public:
  struct LoggerConfig {
    CVarInt verbosityMask{"verbosity_mask", ~0u};
    CVarInt channelMask{"channel_mask", ~0u};
    CVarInt breakOnError{"break_on_error", ~0u};
    CVarInt bytesToBuffer{"bytesToBuffer", 4096};
  };

  // static uint8_t gVerbosityMask;
  // static uint32_t gChannelMask;
  // static bool gAlwaysFlush, gBreakOnError;
  Logger();

  static int DebugPrintF(const std::string file, const int line,
                         const Debug::Channel::Enum channel,
                         const Debug::Verbosity::Enum verbosity,
                         const std::string format, va_list argList);

 protected:
  static int VDebugPrintF(const Debug::Channel::Enum channel,
                          const Debug::Verbosity::Enum verbosity,
                          const std::string format, va_list argList);

 private:
  static std::string engineFileName;
  static std::string channelFileName;
  static std::ostringstream engineStream;
  static std::ostringstream channelStream;

  static bool CheckChannelMask(const Debug::Channel::Enum channel);
  static bool CheckVerbosity(const Debug::Verbosity::Enum verbosity);
  static void BufferWrite(const std::string fileName,
                          std::ostringstream* stream, const char* buffer);
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

  void operator()(const Debug::Channel::Enum channel, const char* inFormat,
                  ...) const;
  void operator()(const Debug::Channel::Enum channel,
                  const std::string& inFormat) const;
  void operator()(const Debug::Channel::Enum channel,
                  const std::initializer_list<std::string>& inFormat) const;
};
}  // namespace Isetta
