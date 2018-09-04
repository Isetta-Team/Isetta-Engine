#include <iostream>

#include "Debug.h"

namespace Isetta {
    Debug::Verbosity Debug::gVerbosity = Debug::Verbosity::Info;
    uint32_t Debug::gChannelMask = ~0u;
    bool Debug::gAlwaysFlush = false;

    Debug::Verbosity Debug::defaultVerbosity = Verbosity::Info;
    Debug::Channel Debug::defaultChannel = Channel::General;
    std::ofstream Debug::fout;

    int Debug::VDebugPrintF(const bool outputDebug, const std::string format, va_list argList)
    {
        const uint32_t MAX_CHARS = 1023;
        static char s_buffer[MAX_CHARS + 1];
        const std::string m_format = format + '\n';
        int charsWritten = vsnprintf(s_buffer, MAX_CHARS, m_format.c_str(), argList);
        s_buffer[MAX_CHARS] = '\0';

        if (outputDebug)
        {
            OutputDebugString(s_buffer);
        }
        fout << s_buffer;

        if (gAlwaysFlush)
        {
            fout.flush();
        }

        return charsWritten;
    }

    int Debug::PrintF(const std::string format, ...)
    {
        va_list argList;
        va_start(argList, format);

        const std::string m_format = "[TIME][General] " + format;

        int charsWritten = VDebugPrintF(
            CheckChannelMask(defaultChannel) && CheckVerbosity(defaultVerbosity),
            m_format, argList);

        va_end(argList);

        return charsWritten;
    }

    int Debug::PrintF(const Channel channel, const std::string format, ...)
    {
        va_list argList;
        va_start(argList, format);

        const std::string m_format = "[TIME][" + to_string(channel) + "] " + format;

        int charsWritten = VDebugPrintF(
            CheckChannelMask(channel) && CheckVerbosity(defaultVerbosity),
            m_format, argList);

        va_end(argList);

        return charsWritten;
    }

    int Debug::PrintF(const Channel channel, const Verbosity verbosity, const std::string format, ...)
    {
        va_list argList;
        va_start(argList, format);

        const std::string m_format = "[TIME][" + to_string(channel) + "] " + format;

        int charsWritten = VDebugPrintF(
            CheckChannelMask(channel) && CheckVerbosity(verbosity),
            m_format, argList);

        va_end(argList);

        return charsWritten;
    }

    void Debug::LogInfo(const Channel channel, const std::string format, ...)
    {
        va_list argList;
        va_start(argList, format);

        const std::string m_format = "[TIME][" + to_string(channel) + "] " + format;

        int charsWritten = VDebugPrintF(
            CheckChannelMask(channel) && CheckVerbosity(Verbosity::Info),
            m_format, argList);

        va_end(argList);
    }

    void Debug::LogWarning(const Channel channel, const std::string format, ...)
    {
        va_list argList;
        va_start(argList, format);

        const std::string m_format = "[TIME][" + to_string(channel) + "] " + format;

        int charsWritten = VDebugPrintF(
            CheckChannelMask(channel) && CheckVerbosity(Verbosity::Warning),
            m_format, argList);

        va_end(argList);
    }

    void Debug::LogError(const Channel channel, const std::string format, ...)
    {
        va_list argList;
        va_start(argList, format);

        const std::string m_format = "[TIME][" + to_string(channel) + "] " + format;

        int charsWritten = VDebugPrintF(
            CheckChannelMask(channel) && CheckVerbosity(Verbosity::Error),
            m_format, argList);

        va_end(argList);
    }

    void Debug::StartUp()
    {
        // TODO PATH + Timestamp
        fout.open("Debug/debug-log.log");

        // TODO READ FROM ENGINE CONFIG
       /* gVerbosity = Verbosity::Info;
        gChannelMask = ~0u;
        gAlwaysFlush = false;

        defaultVerbosity = Verbosity::Info;
        defaultChannel = Channel::General;*/
    }

    void Debug::ShutDown()
    {
        fout.flush();
        fout.close();
    }

    bool Debug::CheckChannelMask(const Channel channel)
    {
        return (gChannelMask & channel) == channel;
    }

    bool Debug::CheckVerbosity(const Verbosity verbosity)
    {
        return gVerbosity >= verbosity;
    }
}
