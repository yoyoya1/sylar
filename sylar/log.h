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
#include <stdarg.h>
 
/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
 
#define SYLAR_LOG_LEVEL(logger, level) \
	if (logger->getLevel() <= level) \
		sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
			__FILE__, __LINE__, 0, sylar::GetThreadId(), \
		sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)


/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
	if (logger->getLevel() <= level) \
		sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
			__FILE__, __LINE__, 0, sylar::GetThreadId(), \
		sylar::GetFiberId(), time(0), sylar::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)
 
/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)
 
/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)
 
/**
 * @brief 获取主日志器
 */
 #define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

namespace sylar {

class Logger;
class LoggerManger;

/**
 * @brief 日志事件
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
   /**
	* @brief 构造函数
	* @param[in] logger 	 日志器
	* @param[in] level  	 日志级别
	* @param[in] file   	 文件名
	* @param[in] line   	 文件行号
	* @param[in] elapse 	 程序启动时依赖的耗时(毫秒)
	* @param[in] thread_id 	 线程id
	* @param[in] fiber_id  	 协程id
	* @param[in] time        日志事件(s)
	* @param[in] thread_name 线程名称
    */
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
			, const char* file, int32_t line, uint32_t elapse
			, uint32_t thread_id, uint32_t fiber_id, uint64_t time
			, const std::string& thread_name);

	/**
 	* @brief 返回文件名
 	*/
    const char* getFile() const { return m_file; }
	
	/**
 	* @brief 返回行号
 	*/
    int32_t getLine() const { return m_line; }
	
	/**
 	* @brief 返回耗时
 	*/
    uint32_t getElapse() const { return m_elapse; }

	/**
 	* @brief 返回线程id
 	*/
    uint32_t getThreadId() const { return m_threadId; }

	/**
 	* @brief 返回协程id
 	*/
    uint32_t getFiberId() const { return m_fiberId; }

	/**
 	* @brief 返回时间 
 	*/
    uint64_t getTime() const { return m_time; }

	/**
 	* @brief 返回线程名称
 	*/
	const std::string& getThreadName() const { return m_threadName; }

	/**
 	* @brief 返回日志内容
 	*/
    const std::string& getContent() { return m_content; }

	std::shared_ptr<Logger> getLogger() const { return m_logger; }
	
	LogLevel::Level getLevel() const { return m_level; }

	std::stringstream& getSS() { return m__ss; }
	
	void format(const char* fmt, ...);

	void format(const char* fmt, va_list al);
private:
	/// 文件名
    const char* m_file = nullptr; 
	/// 行号
    int32_t m_line = 0;          
	/// 程序启动开始到现在的毫秒数
    uint32_t m_elapse = 0;        
	/// 线程id
    uint32_t m_threadId = 0;      
	/// 协程id
    uint32_t m_fiberId = 0;       
	/// 时间戳
    uint64_t m_time;               
	/// 线程名称
	std::string m_threadName;
	/// 日志内容流
	std::stringstream m_ss;
	/// 日志器
	std::shared_ptr<Logger> m_logger;
	/// 日志等级
	LogLevel::Level m_level;
};


/**
 * @brief 日志级别 
 */

class LogLevel {
public:
    enum Level {
	// 未知级别
        UNKNOW = 0,
	// DEBUG 级别
        DEBUG = 1,
	// INFO 级别
        INFO = 2,
	// WARN 级别
        WARN = 3,
	// ERROR 级别
        ERROR = 4,
	// FATAL 级别
        FATAL = 5
    };
     
	/**
 	* @brief 将日志级别转换成文本输出 
 	*/
    	static const char* ToString(LogLevel::Level);
					
	/**
 	* @brief 将文本输出转换成日志级别
 	*/
	static LogLevel::Level FronmString(const std::string& str);
};
 
/**
 * @brief 日志事件包装器
 */
 
class LogEventWrap {
public:
	
	/**
	 * @brief 构造函数
	 * @param[in] e 日志事件
	 */
	LogEventWrap(LogEvent::ptr e);

	/**
	 * @brief 析构函数
	 */
	~LogEventWrap();

	/**
	 * @brief 获取日志事件
	 */
	LogEvent::ptr getEvent() const { return m_event; }
	
	/**
	 * @brief 获取日志内容流
	 */
	std::stringstream& getSS();
private:

	/**
	 * @brief 日志事件
	 */
	LogEvent::ptr m_event;
};

/**
 * @brief 日志格式化
 */

class LogFormatter {
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	/**
	 * @brief 构造函数
 	 * param[in] pattern 格式模版
	 * @details 
	 * %m 消息
	 * %p 日志级别
	 * %r 累计毫秒数
	 * %c 日志名称
	 * %t 线程id
 	 * %n 换行
	 * %d 时间
 	 * %f 文件名
	 * %l 行号
	 * %T 制表符
	 * %F 协程id
	 * %N 线程名称
	 *
	 * 默认格式 “%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
 	 *
	 */
   	LogFormatter(const std::string& pattern);

	/**
	 * @brief 返回格式化日志文本
	 * @param[in] logger 日志器
	 * @param[in] level 日志级别
	 * @param[in] event 日志事件
	 */
		
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
 	std::ostream& format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
 
 	/** 
 	 * @brief 日志内容项格式化
	 */

    class FormatItem {
    public:
    typedef std::shared_ptr<FormatItem> ptr;
 	/** 
 	 * @brief 析构函数
	 */
    virtual ~FormatItem() {}
	/**
	 * @brief 格式化日志到流
 	 * @param[in, out] os 日志输出流
	 * @param[in] logger 日志器
	 * @param[in] level 日志等级
	 * @param[in] event 日志事件
	 */
    virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };

 	/** 
 	 * @brief 初始化，解析日志模板
	 */
    void init();

 	/** 
 	 * @brief 是否有错误 
	 */
	bool isError() const { return m_error; }

 	/** 
 	 * @brief 返回日志模板
	 */
	const std::string getPattern() const { return m_pattern; }
private:
	/// 日志格式模板
    std::string m_pattern;
	/// 日志格式解析后格式
    std::vector<FormatItem::ptr> m_item;
 	/// 是否有错误
	bool m_error = false;
};

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
