#include "logger.h"

namespace ScmAudio
{

thread_local Logger::Writer Logger::_writer;

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
    : _threadRunning(true)
    , _logLevel(LogLevel::Debug)
    , _logCategoryMask(LogCategory::All)
    , _loggingFunction(std::bind(&Logger::DefaultLoggingFunction, this, std::placeholders::_1))
{
    _thread = Thread(&Logger::ThreadFunction, this);
}

Logger::~Logger()
{
    _threadRunning = false;
    _thread.join();
}

void Logger::SetLoggingFunction(LoggingFunction&& loggingFunction)
{
    if (loggingFunction != nullptr)
        _loggingFunction = loggingFunction;
}

void Logger::SetLogLevel(LogLevel logLevel)
{
    _logLevel = logLevel;
}

void Logger::SetLogCategory(LogCategory logCategory, bool activation)
{
    if (activation)
        _logCategoryMask = static_cast<LogCategory>(static_cast<U32>(_logCategoryMask) | static_cast<U32>(logCategory));
    else
        _logCategoryMask = static_cast<LogCategory>(static_cast<U32>(_logCategoryMask) & ~(static_cast<U32>(logCategory)));
}

bool Logger::LogLevelFilter(LogLevel logLevel)
{
    return static_cast<U32>(logLevel) >= static_cast<U32>(_logLevel);
}

bool Logger::LogCategoryFilter(LogCategory logCategory)
{
    return static_cast<U32>(logCategory) & static_cast<U32>(_logCategoryMask);
}

void Logger::Register(Writer* writer)
{
    if (writer != nullptr)
    {
        ScopedLock<Mutex> lock(_writersLock);
        _writers[writer->threadId] = writer;
    }
}

void Logger::Unregister(Writer* writer)
{
    if (writer != nullptr)
    {
        ScopedLock<Mutex> lock(_writersLock);
        _writers.erase(writer->threadId);
    }
}

void Logger::DefaultLoggingFunction(const Entry& entry)
{
    std::cout
        //<< entry.timestamp << '\t'
        //<< entry.threadId << '\t'
        << LogLevelString(entry.level) << '\t'
        << LogCategoryString(entry.category) << '\t'
        << entry.message << '\n';
}

void Logger::ThreadFunction()
{
    Entry tmpEntry;
    Set<Entry> entries;
    while (_threadRunning)
    {
        {
            ScopedLock<Mutex> lock(_writersLock);

            for (auto& item : _writers)
            {
                Writer& writer = *(item.second);
                while (writer.entries.try_dequeue(tmpEntry))
                    entries.insert(tmpEntry);
            }
        }

        for (const Entry& entry : entries)
            _loggingFunction(entry);

        entries.clear();
        Sleep(1);
    }
}

// Writer functions

Logger::Writer::Writer()
    : threadId(Hash(std::this_thread::get_id()))
    , entries(64)
{
    Logger::GetInstance().Register(this);
}

Logger::Writer::~Writer()
{
    Logger::GetInstance().Unregister(this);
}

void Logger::Writer::Write(LogLevel level, LogCategory category, const String& message)
{
    Entry entry;
    entry.timestamp = TimeNow();
    entry.threadId = threadId;
    entry.level = level;
    entry.category = category;
    entry.message = message;
    entries.wait_enqueue(entry);
}

} // namespace ScmAudio
