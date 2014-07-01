#pragma once
#include <stdint.h>
#include <vector>
#include <sstream>
#include "utils.hpp"

namespace bristol
{
  enum class LogLevel
  {
    None,
    Debug,
    Info,
    Warning,
    Error,
  };

  struct LogEntry
  {
    const char* file;
    uint32_t line;
    std::string desc;
    std::vector<std::pair<std::string, std::string>> values;
  };

  //----------------------------------------------------------------------------------
  template <typename T>
  struct LogObject
  {
    LogObject(const std::string& key, const T& value) : key(key), value(value) {}
    std::string key;
    T value;
  };

  //----------------------------------------------------------------------------------
  template <typename T>
  LogObject<T> LogKeyValue(const std::string& key, const T& value)
  {
    return LogObject<T>(key, value);
  }

  //----------------------------------------------------------------------------------
  struct LogSink
  {
    LogSink();
    virtual ~LogSink();
    virtual void Log(LogLevel level, const LogEntry& entry) = 0;
  };

  //----------------------------------------------------------------------------------
  struct LogSinkFile : public LogSink
  {
    LogSinkFile();
    ~LogSinkFile();

    bool Open(const char* filename);
    virtual void Log(LogLevel level, const LogEntry& entry);

    FILE* _file;
  };

  //----------------------------------------------------------------------------------
  struct LogSinkConsole : public LogSink
  {
    virtual void Log(LogLevel level, const LogEntry& entry);
  };

  //----------------------------------------------------------------------------------
  struct LogSinkApp : public LogSink
  {
    virtual void Log(LogLevel level, const LogEntry& entry);
  };

  //----------------------------------------------------------------------------------
  struct LogStream
  {
    LogStream(LogLevel level, const char* file, uint32_t line);
    ~LogStream();

    void Append(const std::string& key, const std::string& value);

    LogEntry _entry;
    LogLevel _level;
  };

  template <typename T>
  LogStream& operator<<(LogStream& s, const LogObject<T>& lhs)
  {
    std::ostringstream str;
    str << lhs.value;
    s.Append(lhs.key, str.str());
    return s;
  }

  LogStream& operator<<(LogStream& s, const char* desc);

  // The minimum level at which we log
  LogLevel GetLogLevel();
  void SetLogLevel(LogLevel level);

#define LOG_DEBUG(x)  \
    if (GetLogLevel() <= bristol::LogLevel::Debug) {  \
      bristol::LogStream GEN_NAME(s, __LINE__)(bristol::LogLevel::Debug, __FILE__, __LINE__); GEN_NAME(s, __LINE__) << x  \
    }

#define LOG_INFO(x) \
  bristol::LogStream GEN_NAME(s, __LINE__)(bristol::LogLevel::Info, __FILE__, __LINE__); GEN_NAME(s, __LINE__) << x
#define LOG_WARN(x) \
  bristol::LogStream GEN_NAME(s, __LINE__)(bristol::LogLevel::Warning, __FILE__, __LINE__); GEN_NAME(s, __LINE__) << x
#define LOG_ERROR(x)  \
  bristol::LogStream GEN_NAME(s, __LINE__)(bristol::LogLevel::Error, __FILE__, __LINE__); GEN_NAME(s, __LINE__) << x
}
