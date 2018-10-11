/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Debug/Logger.h"

#include <sstream>
#include <type_traits>
#include "Core/Config/Config.h"
#include "Core/Debug/Assert.h"
#include "Core/Filesystem.h"
#include "Core/Time/Clock.h"

namespace Isetta {
std::string Logger::engineFileName;
std::string Logger::channelFileName;
std::ostringstream Logger::engineStream;
std::ostringstream Logger::channelStream;

void Logger::NewSession() {
  std::string folder = "";
  if (!Config::Instance().logger.logFolder.GetVal().empty()) {
    Filesystem::Concat({Config::Instance().logger.logFolder.GetVal()}, &folder);
  }
  std::string timestamp = std::to_string(Clock::GetTimestamp());
  engineFileName = folder + "isetta-log_" + timestamp + ".log";
  channelFileName = folder + "isetta-channel-log_" + timestamp + ".log";
  Filesystem::Instance().Touch(channelFileName);
  Filesystem::Instance().Touch(engineFileName);
}

int Logger::VDebugPrintF(const Debug::Channel channel,
                         const Debug::Verbosity verbosity,
                         const std::string inFormat, va_list argList) {
  const U32 MAX_CHARS = 1023;
  static char sBuffer[MAX_CHARS + 1];
  // TODO(Jacob) elapsed or unscaled time?
  std::ostringstream stream;
  stream << "[" << EngineLoop::GetGameClock().GetElapsedUnscaledTime() << "]["
         << ToString(verbosity) << "][" << ToString(channel) << "] " << inFormat
         << '\n';
  int charsWritten =
      vsnprintf(sBuffer, MAX_CHARS, stream.str().c_str(), argList);

  sBuffer[charsWritten] = '\0';

  if (CheckChannelMask(channel)) {
    if (CheckVerbosity(verbosity)) {
      OutputDebugString(sBuffer);
      BufferWrite(channelFileName, &channelStream, sBuffer);
    }
  }

  BufferWrite(engineFileName, &engineStream, sBuffer);

  if (Config::Instance().logger.breakOnError.GetVal() &&
      verbosity == Debug::Verbosity::Error &&
      CheckVerbosity(Debug::Verbosity::Error)) {
    ASSERT(false);
  }

  return charsWritten;
}

int Logger::DebugPrintF(const std::string file, const int line,
                        const Debug::Channel channel,
                        const Debug::Verbosity verbosity,
                        const std::string inFormat, va_list argList) {
  std::ostringstream stream;
  stream << file << "(" << line << ") " << inFormat;

  int charsWritten = VDebugPrintF(channel, verbosity, stream.str(), argList);

  return charsWritten;
}

bool Logger::CheckChannelMask(const Debug::Channel channel) {
  typedef std::underlying_type<Debug::Channel>::type utype;
  return (Config::Instance().logger.channelMask.GetVal() &
          static_cast<utype>(channel)) == static_cast<utype>(channel);
}

bool Logger::CheckVerbosity(const Debug::Verbosity verbosity) {
  typedef std::underlying_type<Debug::Verbosity>::type utype;
  return (Config::Instance().logger.verbosityMask.GetVal() &
          static_cast<utype>(verbosity)) == static_cast<utype>(verbosity);
}

void Logger::BufferWrite(const std::string fileName, std::ostringstream* stream,
                         const char* buffer) {
  *stream << buffer;
  stream->seekp(0, std::ios::end);
  if (static_cast<int>(stream->tellp()) >=
      Config::Instance().logger.bytesToBuffer.GetVal()) {
    stream->flush();
    Filesystem::Instance().WriteAsync(fileName, stream->str());
    stream->str("");
    stream->clear();
    stream->seekp(0, std::ios::beg);
  }
}

void LogObject::operator()(const Debug::Channel channel,
                           const Debug::Verbosity verbosity,
                           const char* inFormat, ...) const {
  va_list argList;
  va_start(argList, &inFormat);

  Logger::DebugPrintF(file, line, channel, verbosity, inFormat, argList);

  va_end(argList);
}

void LogObject::operator()(const Debug::Channel channel,
                           const Debug::Verbosity verbosity,
                           const std::string& inFormat) const {
  Logger::DebugPrintF(file, line, channel, verbosity, inFormat.c_str(), NULL);
}

void LogObject::operator()(
    const Debug::Channel channel, const Debug::Verbosity verbosity,
    const std::initializer_list<std::string>& inFormat) const {
  std::string stream;
  for (auto& elem : inFormat) {
    stream += elem;
  }
  Logger::DebugPrintF(file, line, channel, verbosity, stream.c_str(), NULL);
}

void LogObject::operator()(const Debug::Channel channel, const char* inFormat,
                           ...) const {
  va_list argList;
  va_start(argList, &inFormat);

  Logger::DebugPrintF(file, line, channel, verbosity, inFormat, argList);

  va_end(argList);
}

void LogObject::operator()(const Debug::Channel channel,
                           const std::string& inFormat) const {
  Logger::DebugPrintF(file, line, channel, verbosity, inFormat.c_str(), NULL);
}

void LogObject::operator()(
    const Debug::Channel channel,
    const std::initializer_list<std::string>& inFormat) const {
  std::string stream;
  for (auto& elem : inFormat) {
    stream += elem;
  }
  Logger::DebugPrintF(file, line, channel, verbosity, stream.c_str(), NULL);
}
}  // namespace Isetta
