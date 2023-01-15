#ifndef SYLAR_LOG_H
#define SYLAR_LOG_H

#include <string>
#include <stdint.h>
#include <memory>
#include <list>

namespace sylar {

// LogEvent

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
private:
    const char* m_file = nullptr; // FileName
    int32_t m_line = 0;           // RowNumber
    uint32_t m_elapse = 0;        // The milliseconds since the program was started
    uint32_t m_threadId = 0;      // threadID
    uint32_t m_fiberId = 0;       // fiberID
    uint64_t m_time;              // TimeStamp 
    std::string m_content;
};

// Log Level

class LogLevel {
public:
    enum Level {
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};
 
// LogFormatter 

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
     
    std::string format(LogEvent::ptr event);
private:
};

// Log Output Place

class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}
     
    void log(LogLevel::Level level, LogEvent::ptr event);
private:
    LogLevel::Level m_level;
};

// Logger

class Logger {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
private:
    std::string m_name;                       // Log Name
    LogLevel::Level m_level;                  // Log Level
    std::list<LogAppender::ptr> m_appenders;  // Log AppenderSet
};

// Output stdout Appender

class StdoutLogAppender : public LogAppender {
};
 
// Output file Appender  

class FileLogAppender : public LogAppender {
};

}

#endif