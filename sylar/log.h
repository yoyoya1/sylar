#ifndef SYLAR_LOG_H
#define SYLAR_LOG_H

#include <string>
#include <iostream>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>

namespace sylar {

class Logger;
// LogEvent

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

    const char* getFile() const { return m_file; }
    int32_t getLine() const { return m_line; }
    uint32_t getElapse() const { return m_elapse; }
    uint32_t getThreadId() const { return m_threadId; }
    uint32_t getFiberId() const { return m_fiberId; }
    uint64_t getTime() const { return m_time; }
    const std::string& getContent() { return m_content; }
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
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
     
    static const char* ToString(LogLevel::Level);
};

// LogAppender

class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}
     
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
    LogFormatter::ptr getFormatter() const { return m_formatter; }
protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};

// LogOutPlace

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

    class FormatItem {
    public:
    typedef std::shared_ptr<FormatItem> ptr;
    virtual ~FormatItem() {}
    virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };

    void init();
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_item;
};

// Logger

class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
     
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }
    
    const std::string& getName() { return m_name; }
private:
    std::string m_name;                       // Log Name
    LogLevel::Level m_level;                  // Log Level
    std::list<LogAppender::ptr> m_appenders;  // Log AppenderSet
    LogFormatter::ptr m_formatter;
};

// Output stdout Appender

class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
private:
};
 
// Output file Appender  

class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
     
    FileLogAppender(const std::string& filename);
    bool reopen() {}
private:
    std::string m_filename;
    std::ofstream m_filestream;
};

}

#endif
 
// Use
// (FileName)        -> LogEvent
// (LogEvent)        -> Logger(User, some Appenders)
// (StdOutAppender)  -> log function(Event) -> StdOut
// (FileOutAppender) -> log function(Event) -> m_FileStream