/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Debug.h"

namespace Isetta {
Debug::Verbosity Debug::gVerbosity = Debug::Verbosity::Info;
uint32_t Debug::gChannelMask = ~0u;
bool Debug::gAlwaysFlush = false;

Debug::Verbosity Debug::defaultVerbosity = Verbosity::Info;
Debug::Channel Debug::defaultChannel = Channel::General;
std::ofstream Debug::fout;

int Debug::VDebugPrintF(const bool outputDebug, const std::string inFormat,
                        va_list argList) {
  const uint32_t MAX_CHARS = 1023;
  static char sBuffer[MAX_CHARS + 1];
  const std::string format = inFormat + '\n';
  int charsWritten = vsnprintf(sBuffer, MAX_CHARS, format.c_str(), argList);
  sBuffer[MAX_CHARS] = '\0';

  if (outputDebug) {
    OutputDebugString(sBuffer);
  }
  fout << sBuffer;

  if (gAlwaysFlush) {
    fout.flush();
  }

  return charsWritten;
}

int Debug::PrintF(const std::string inFormat, ...) {
  va_list argList;
  va_start(argList, &inFormat);

  const std::string format = "[TIME][General] " + inFormat;

  int charsWritten = VDebugPrintF(
      CheckChannelMask(defaultChannel) && CheckVerbosity(defaultVerbosity),
      format, argList);

  va_end(argList);
  return charsWritten;
}

int Debug::PrintF(const Channel channel, const std::string inFormat, ...) {
  va_list argList;
  va_start(argList, &inFormat);

  const std::string format = "[TIME][" + ToString(channel) + "] " + inFormat;

  int charsWritten = VDebugPrintF(
      CheckChannelMask(channel) && CheckVerbosity(defaultVerbosity), format,
      argList);

  va_end(argList);

  return charsWritten;
}

int Debug::PrintF(const Channel channel, const Verbosity verbosity,
                  const std::string inFormat, ...) {
  va_list argList;
  va_start(argList, &inFormat);

  const std::string format = "[TIME][" + ToString(channel) + "] " + inFormat;

  int charsWritten = VDebugPrintF(
      CheckChannelMask(channel) && CheckVerbosity(verbosity), format, argList);

  va_end(argList);

  return charsWritten;
}

void Debug::LogInfo(const Channel channel, const std::string inFormat, ...) {
  va_list argList;
  va_start(argList, &inFormat);

  const std::string format = "[TIME][" + ToString(channel) + "] " + inFormat;

  VDebugPrintF(CheckChannelMask(channel) && CheckVerbosity(Verbosity::Info),
               format, argList);

  va_end(argList);
}

void Debug::LogWarning(const Channel channel, const std::string inFormat, ...) {
  va_list argList;
  va_start(argList, &inFormat);

  const std::string format = "[TIME][" + ToString(channel) + "] " + inFormat;

  VDebugPrintF(CheckChannelMask(channel) && CheckVerbosity(Verbosity::Warning),
               format, argList);

  va_end(argList);
}

void Debug::LogError(const Channel channel, const std::string inFormat, ...) {
  va_list argList;
  va_start(argList, &inFormat);

  const std::string format = "[TIME][" + ToString(channel) + "] " + inFormat;

  VDebugPrintF(CheckChannelMask(channel) && CheckVerbosity(Verbosity::Error),
               format, argList);

  va_end(argList);
}

void Debug::StartUp() {
  // TODO PATH + Timestamp
  fout.open("Debug/debug-log.log");

  // TODO READ FROM ENGINE CONFIG
  /* gVerbosity = Verbosity::Info;
   gChannelMask = ~0u;
   gAlwaysFlush = false;

   defaultVerbosity = Verbosity::Info;
   defaultChannel = Channel::General;*/
}

void Debug::ShutDown() {
  fout.flush();
  fout.close();
}

bool Debug::CheckChannelMask(const Channel channel) {
  return (gChannelMask & channel) == static_cast<uint32_t>(channel);
}

bool Debug::CheckVerbosity(const Verbosity verbosity) {
  return gVerbosity >= verbosity;
}
}  // namespace Isetta
