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
#include "Core/Debug/Debug.h"

namespace Isetta {
#define LOG LogObject(__FILENAME__, __LINE__)
#define LOG_INFO LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Info)
#define LOG_WARNING LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Warning)
#define LOG_ERROR LogObject(__FILENAME__, __LINE__, Debug::Verbosity::Error)

class Logger {
 public:
  struct LoggerConfig {
    CVar<uint8_t> verbosityMask{"verbosity_mask", ~0u};
    CVar<uint32_t> channelMask{"channel_mask", ~0u};
    CVar<int> breakOnError{"break_on_error", 1};
    CVar<int> bytesToBuffer{"bytesToBuffer", 10000};
  };

  static void NewSession();
  static int DebugPrintF(const std::string file, const int line,
                         const Debug::Channel channel,
                         const Debug::Verbosity verbosity,
                         const std::string format, va_list argList);

 protected:
  static int VDebugPrintF(const Debug::Channel channel,
                          const Debug::Verbosity verbosity,
                          const std::string format, va_list argList);

 private:
  static std::string engineFileName;
  static std::string channelFileName;
  static std::ostringstream engineStream;
  static std::ostringstream channelStream;

  static bool CheckChannelMask(const Debug::Channel channel);
  static bool CheckVerbosity(const Debug::Verbosity verbosity);
  static void BufferWrite(const std::string fileName,
                          std::ostringstream* stream, const char* buffer);
};  // namespace Isetta

struct LogObject {
  std::string file;
  int line;
  Debug::Verbosity verbosity = Debug::Verbosity::Info;

  LogObject(const std::string file, const int line) : file{file}, line{line} {}
  LogObject(const std::string file, const int line, Debug::Verbosity verbosity)
      : file{file}, line{line}, verbosity{verbosity} {}

  void operator()(const Debug::Channel channel,
                  const Debug::Verbosity verbosity, const char* inFormat,
                  ...) const;
  void operator()(const Debug::Channel channel,
                  const Debug::Verbosity verbosity,
                  const std::string& inFormat) const;
  void operator()(const Debug::Channel channel,
                  const Debug::Verbosity verbosity,
                  const std::initializer_list<std::string>& inFormat) const;

  void operator()(const Debug::Channel channel, const char* inFormat,
                  ...) const;
  void operator()(const Debug::Channel channel,
                  const std::string& inFormat) const;
  void operator()(const Debug::Channel channel,
                  const std::initializer_list<std::string>& inFormat) const;
};
}  // namespace Isetta
