/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Debug/Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "Core/Config/Config.h"
#include "Core/Debug/Assert.h"
#include "Core/FileSystem.h"

namespace Isetta {
std::string Logger::engineFileName = "isetta-log.log";
std::string Logger::channelFileName = "isetta-channel-log.log";
std::ostringstream Logger::engineStream;
std::ostringstream Logger::channelStream;

Logger::Logger() {
  FileSystem::Instance().Write(channelFileName, "", nullptr, false);
  FileSystem::Instance().Write(engineFileName, "", nullptr, false);
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
    // BufferWrite(channelFileName, &channelStream, sBuffer);
  }

  // BufferWrite(engineFileName, &engineStream, sBuffer);

  if (Config::Instance().logger.breakOnError.GetVal() &&
      verbosity == Debug::Verbosity::Error &&
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
  return (Config::Instance().logger.channelMask.GetVal() & channel) ==
         static_cast<uint32_t>(channel);
}

bool Logger::CheckVerbosity(const Debug::Verbosity::Enum verbosity) {
  return (Config::Instance().logger.verbosityMask.GetVal() & verbosity) ==
         static_cast<uint8_t>(verbosity);
}

void Logger::BufferWrite(const std::string fileName, std::ostringstream* stream,
                         const char* buffer) {
  stream->write(buffer, strlen(buffer));
  stream->seekp(0, std::ios::end);
  if (static_cast<int>(stream->tellp()) >=
      Config::Instance().logger.bytesToBuffer.GetVal()) {
    stream->flush();
    FileSystem::Instance().Write(fileName, stream->str());
    stream->str("");
    stream->clear();
    stream->seekp(0, std::ios::beg);
  }
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
