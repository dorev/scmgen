#pragma once

#include "types.h"
#include "utils.h"
#include "defines.h"
#include <iostream>
#include <functional>
#include <sstream>

namespace ScmAudio
{

#define LOG(level, category, message, ...) Logger::GetInstance().Log(LogLevel::level, LogCategory::category, message, __VA_ARGS__)
#define LOG_AUDIO_ERROR(message, ...) LOG(Error, Audio, message, __VA_ARGS__)
#define LOG_FFT_ERROR(message, ...) LOG(Error, Fft, message, __VA_ARGS__)

enum class LogLevel
{
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3,
    Fatal = 4,
    Off = 5
};

inline const char* LogLevelString(LogLevel logLevel)
{
    switch (logLevel)
    {
        CASE_RETURN_STRING(LogLevel, Debug);
        CASE_RETURN_STRING(LogLevel, Info);
        CASE_RETURN_STRING(LogLevel, Warn);
        CASE_RETURN_STRING(LogLevel, Error);
        CASE_RETURN_STRING(LogLevel, Fatal);
        CASE_RETURN_STRING(LogLevel, Off);
    }
    return "Invalid LogLevel value";
}


enum class LogCategory : U32
{
    Generic = BIT(0),
    Audio = BIT(1),
    Fft = BIT(2),
    All = U32MAX
};

inline const char* LogCategoryString(LogCategory logCategory)
{
    switch (logCategory)
    {
        CASE_RETURN_STRING(LogCategory, Generic);
        CASE_RETURN_STRING(LogCategory, Audio);
        CASE_RETURN_STRING(LogCategory, Fft);
        CASE_RETURN_STRING(LogCategory, All);
    }
    return "Invalid LogCategory value";
}

class Logger
{
private:
    struct Entry;
    struct Writer;

public:

    static Logger& GetInstance();
    ~Logger();
    using LoggingFunction = std::function<void(const Entry&)>;
    void SetLoggingFunction(LoggingFunction&& loggingFunction);
    void SetLogLevel(LogLevel logLevel);
    void SetLogCategory(LogCategory logCategory, bool activation = true);

    template <class... Args>
    void Log(LogLevel logLevel, LogCategory category, Args... message)
    {
        if (LogLevelFilter(logLevel))
        {
            std::stringstream stream;
            ((stream << std::forward<Args>(message)), ...);
            _writer.Write(logLevel, category, stream.str());
        }
    }

private:
    struct Entry
    {
        U64 timestamp;
        U64 threadId;
        LogLevel level;
        LogCategory category;
        String message;
        bool operator<(const Entry& other) const { return timestamp < other.timestamp; }
    };

    friend struct Writer;
    struct Writer
    {
        const U64 threadId;
        LockfreeCircularBuffer<Entry> entries;

        Writer();
        ~Writer();
        void Write(LogLevel level, LogCategory category, const String& message);
    };

    Logger();
    bool LogLevelFilter(LogLevel logLevel);
    bool LogCategoryFilter(LogCategory logCategory);
    void Register(Writer* writer);
    void Unregister(Writer* writer);
    void DefaultLoggingFunction(const Entry& entry);
    void ThreadFunction();

private:
    static thread_local Writer _writer;
    Mutex _writersLock;
    Map<U64, Writer*> _writers;
    LoggingFunction _loggingFunction;
    Thread _thread;
    bool _threadRunning;
    LogLevel _logLevel;
    LogCategory _logCategoryMask;
};


} // namespace ScmAudio