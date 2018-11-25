/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#define __FILENAME__ \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include <Windows.h>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <string>
#include "Core/Config/CVar.h"
#include "Core/Debug/Debug.h"
#include "Core/IsettaAlias.h"
#include "ISETTA_API.h"

namespace Isetta {
#define LOG Isetta::LogObject(__FILENAME__, __LINE__)
#define LOG_INFO \
  Isetta::LogObject(__FILENAME__, __LINE__, Isetta::Debug::Verbosity::Info)
#define LOG_WARNING \
  Isetta::LogObject(__FILENAME__, __LINE__, Isetta::Debug::Verbosity::Warning)
#define LOG_ERROR \
  Isetta::LogObject(__FILENAME__, __LINE__, Isetta::Debug::Verbosity::Error)

/**
 * @brief static class for outputing to the Visual Studio Output window in Debug
 * mode and to log file(s) (1 for all output messages and 1 for specific channel
 * mask messages)
 *
 */
class ISETTA_API Logger {
 public:
  /**
   * @brief Logger configuration CVar struct
   *
   */
  struct LoggerConfig {
    /*
  /// Determines the severity needed for what output messages are shown
  CVar<U8> verbosityMask{"verbosity_mask", ~0u};
  /// Determines the channels which get output and which are written to the
  /// channel file
  CVar<U32> channelMask{"channel_mask", ~0u};
  */
    /// If the engine breaks into a breakpoint with LOG_ERROR
    CVar<int> breakOnError{"break_on_error", 1};
    /// Number of characters/bytes to write before flushing to the output buffer
    CVar<int> bytesToBuffer{"bytes_to_buffer", 10000};
    /// Folder location where the log files are written
    CVarString logFolder{"logger_folder", "Logs"};
  };

  static std::bitset<static_cast<int>(Debug::Channel::All)> channelMask;
  static std::bitset<static_cast<int>(Debug::Verbosity::All) - 1> verbosityMask;

  static Action<const char*> outputCallback;
  /**
   * @brief Checks if the given channel enum is part of the channel mask,
   * masking non-marked channels
   *
   * @param channel of the input message
   * @return true if the channel enum is included in the channel mask
   * @return false if the channel enum is not part of the channel mask
   */
  static bool CheckChannelMask(const Debug::Channel channel);
  /**
   * @brief Checks if the given verbosity enum is part of the verbosity mask,
   * masking non-marked verbosity
   *
   * @param verbosity of the input message
   * @return true if the verbosity enum is included in the verbosity mask
   * @return false if the verbosity enum is not part of the verbosity mask
   */
  static bool CheckVerbosity(const Debug::Verbosity verbosity);

  /**
   * @brief Creates a new session of the logger, which attempts to create a new
   * engine and channel log file in log folder with timestamp.
   *
   */
  static void NewSession();

  static void ShutDown();
  /**
   * @brief The function which parses the input parameters, then calls
   * VDebugPrintF for the actual output. Should default to LOG macros rather
   * than calling this method directly.
   *
   * @param file which file the error is in, can use __FILE__
   * @param line which line the error is on, can use __LINE__
   * @param channel specified which channel the message is associated with
   * @param verbosity specifies how severe the message is
   * @param format follows the same format as prinft function
   * @param argList the arguments to be inserted into the printf string
   * @return int number of characters printed to the output
   */
  static int DebugPrintF(const std::string file, const int line,
                         const Debug::Channel channel,
                         const Debug::Verbosity verbosity,
                         const std::string format, va_list argList);

 protected:
  /**
   * @brief Function which actually outputs to Visual Studio Output as well as
   * channel and engine log files. Formats the message with time, channel, and
   * verbosity. The function filters the verbosity and channel by the set mask
   * to stop from outputting to the window. Only the filtered messages go to the
   * output. All messages go to engine log file, filtered messages go the the
   * channel log file.
   *
   * @param channel specified which channel the message is associated with
   * @param verbosity specifies how severe the message is
   * @param format follows the same format as prinft function
   * @param argList the arguments to be inserted into the printf string
   * @return int number of characters printed to the output
   */
  static int VDebugPrintF(const Debug::Channel channel,
                          const Debug::Verbosity verbosity,
                          const std::string format, va_list argList);

 private:
  /**
   * @brief Construct a new Logger object - cannot instance this object
   *
   */
  Logger() = default;

  /// The file path of the engine log file (with timestamp)
  static std::string engineFileName;
  /// The file path of the channel log file (with timestamp)
  static std::string channelFileName;
  /// The stream containing all the characters before pushing to the engine file
  static std::ostringstream engineStream;
  /// The stream containing the filtered channel characters before pushing to
  /// the channel file
  static std::ostringstream channelStream;

  /**
   * @brief Adds the buffer to the stream if it passes the verbosity and channel
   * checks, then writes to the file.
   *
   * @param fileName of the file to write to if the stream buffer is above the
   * byte threshold
   * @param stream which the buffer is written to, to store information
   * @param buffer which contains the characters of the message (formatted)
   */
  static void BufferWrite(const std::string fileName,
                          std::ostringstream* stream, const char* buffer);
};  // namespace Isetta

/**
 * @brief Helper object for the log macros, so __FILE__ and __LINE__ can be
 * passed by default at the given position
 *
 */
struct ISETTA_API LogObject {
  /// File where the log macro is called from
  std::string file;
  // Line where the log macro is called from
  int line;
  // Default verbosity level if not set
  Debug::Verbosity verbosity = Debug::Verbosity::Info;

  /**
   * @brief Construct a new Log Object object (uses default verbosity)
   *
   * @param file of where the object is called
   * @param line of where the object is called
   */
  LogObject(const std::string file, const int line) : file{file}, line{line} {}
  /**
   * @brief Construct a new Log Object object
   *
   * @param file of where the object is called
   * @param line of where the object is called
   * @param verbosity of how the severe the message is
   */
  LogObject(const std::string file, const int line, Debug::Verbosity verbosity)
      : file{file}, line{line}, verbosity{verbosity} {}

  /**
   * @brief Call to DebugPrintF to output a log message
   *
   * @param channel of the message
   * @param verbosity of the message
   * @param inFormat the format string similar to printf
   * @param ... input arguments to fill the formats
   */
  void operator()(const Debug::Channel channel,
                  const Debug::Verbosity verbosity, const char* inFormat,
                  ...) const;
  /**
   * @brief Call to DebugPrintF to output a log message
   *
   * @param channel of the message
   * @param verbosity of the message
   * @param inFormat the string to output
   */
  void operator()(const Debug::Channel channel,
                  const Debug::Verbosity verbosity,
                  const std::string& inFormat) const;
  /**
   * @brief Call to DebugPrintF to output a log message
   *
   * @param channel of the message
   * @param verbosity of the message
   * @param inFormat initializer list of strings to output
   */
  void operator()(const Debug::Channel channel,
                  const Debug::Verbosity verbosity,
                  const std::initializer_list<std::string>& inFormat) const;

  /**
   * @brief Call to DebugPrintF to output a log message,
   * verbosity set in constructor
   *
   * @param channel of the message
   * @param inFormat the format string similar to printf
   * @param ... input arguments to fill the formats
   */
  void operator()(const Debug::Channel channel, const char* inFormat,
                  ...) const;
  /**
   * @brief Call to DebugPrintF to output a log message,
   * verbosity set in constructor
   *
   * @param channel of the message
   * @param inFormat the string to output
   */
  void operator()(const Debug::Channel channel,
                  const std::string& inFormat) const;
  /**
   * @brief Call to DebugPrintF to output a log message,
   * verbosity set in constructor
   *
   * @param channel of the message
   * @param inFormat initializer list of strings to output
   */
  void operator()(const Debug::Channel channel,
                  const std::initializer_list<std::string>& inFormat) const;

  /**
   * @brief Call to DebugPrintF to output a log message,
   * verbosity set in constructor
   *
   * @param inFormat the format string similar to printf
   * @param ... input arguments to fill the formats
   */
  void operator()(const char* inFormat, ...) const;
  /**
   * @brief Call to DebugPrintF to output a log message,
   * verbosity set in constructor
   *
   * @param inFormat the string to output
   */
  void operator()(const std::string& inFormat) const;
  /**
   * @brief Call to DebugPrintF to output a log message,
   * verbosity set in constructor
   *
   * @param inFormat initializer list of strings to output
   */
  void operator()(const std::initializer_list<std::string>& inFormat) const;
};
}  // namespace Isetta
