/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Debug/Logger.h"

#include <Windows.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Core/Debug/Assert.h"

namespace Isetta {

uint8_t Logger::gVerbosityMask = ~0u;
uint32_t Logger::gChannelMask = ~0u;
bool Logger::gAlwaysFlush = false;
bool Logger::gBreakOnError = false;

std::ofstream* Logger::engineStream = nullptr;
std::ofstream* Logger::channelStream = nullptr;

void Logger::SetLoggerFiles(std::ofstream* inEngineStream,
                            std::ofstream* inChannelStream) {
  Logger::engineStream = inEngineStream;
  if (inChannelStream) {
    Logger::channelStream = inChannelStream;
  }
}
void Logger::SetLoggerChannelFile(std::ofstream* inChannelStream) {
  if (!Logger::engineStream) {
    throw std::logic_error(
        "Logger::SetLoggerChannel must have engine log file before changing "
        "channel file");
  }
  if (Logger::channelStream) {
    Logger::channelStream->close();
  }
  Logger::channelStream = inChannelStream;
}

int Logger::VDebugPrintF(const Debug::Channel::Enum channel,
                         const Debug::Verbosity::Enum verbosity,
                         const std::string inFormat, va_list argList) {
  const uint32_t MAX_CHARS = 1023;
  static char sBuffer[MAX_CHARS + 1];
  // TODO(jacob) add actual time
  std::ostringstream stream;
  stream << "[" << 12 << ":" << 12 << ":" << 12 << "][" << ToString(verbosity)
         << "][" << ToString(channel) << "] " << inFormat << '\n';
  int charsWritten =
      vsnprintf(sBuffer, MAX_CHARS, stream.str().c_str(), argList);

  sBuffer[MAX_CHARS] = '\0';

  if (CheckChannelMask(channel)) {
    if (CheckVerbosity(verbosity)) {
      OutputDebugString(sBuffer);
    }
    if (Logger::channelStream) {
      *Logger::channelStream << sBuffer;
    }
  }
  if (Logger::engineStream) {
    *Logger::engineStream << sBuffer;
  }

  if (gAlwaysFlush) {
    Logger::engineStream->flush();
  }
  if (gBreakOnError && verbosity == Debug::Verbosity::Error &&
      CheckVerbosity(Debug::Verbosity::Error)) {
    ASSERT(false);
  }

  return charsWritten;
}

int Logger::DebugPrintF(const std::string file, const int line,
                        const Debug::Channel::Enum channel,
                        const Debug::Verbosity::Enum verbosity,
                        const std::string inFormat, va_list argList) {
  std::ostringstream stream;
  stream << file << "(" << line << ") " << inFormat;

  int charsWritten = VDebugPrintF(channel, verbosity, stream.str(), argList);

  return charsWritten;
}

bool Logger::CheckChannelMask(const Debug::Channel::Enum channel) {
  return (gChannelMask & channel) == static_cast<uint32_t>(channel);
}

bool Logger::CheckVerbosity(const Debug::Verbosity::Enum verbosity) {
  return (gVerbosityMask & verbosity) == static_cast<uint8_t>(verbosity);
}
void LogObject::operator()(const Debug::Channel::Enum channel,
                           const Debug::Verbosity::Enum verbosity,
                           const char* inFormat, ...) const {
  va_list argList;
  va_start(argList, &inFormat);

  Logger::DebugPrintF(file, line, channel, verbosity, inFormat, argList);

  va_end(argList);
}

void LogObject::operator()(const Debug::Channel::Enum channel,
                           const Debug::Verbosity::Enum verbosity,
                           const std::string& inFormat) const {
  Logger::DebugPrintF(file, line, channel, verbosity, inFormat.c_str(), NULL);
}

void LogObject::operator()(
    const Debug::Channel::Enum channel, const Debug::Verbosity::Enum verbosity,
    const std::initializer_list<std::string>& inFormat) const {
  std::ostringstream stream;
  stream << file << "(" << line << ") ";
  for (auto& elem : inFormat) {
    stream << elem;
  }
  Logger::DebugPrintF(file, line, channel, verbosity, stream.str().c_str(),
                      NULL);
}

void LogObject::operator()(const Debug::Channel::Enum channel,
                           const char* inFormat, ...) const {
  va_list argList;
  va_start(argList, &inFormat);

  Logger::DebugPrintF(file, line, channel, verbosity, inFormat, argList);

  va_end(argList);
}

void LogObject::operator()(const Debug::Channel::Enum channel,
                           const std::string& inFormat) const {
  Logger::DebugPrintF(file, line, channel, verbosity, inFormat.c_str(), NULL);
}

void LogObject::operator()(
    const Debug::Channel::Enum channel,
    const std::initializer_list<std::string>& inFormat) const {
  std::ostringstream stream;
  stream << file << "(" << line << ") ";
  for (auto& elem : inFormat) {
    stream << elem;
  }
  Logger::DebugPrintF(file, line, channel, verbosity, stream.str().c_str(),
                      NULL);
}
}  // namespace Isetta
