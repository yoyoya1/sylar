#include "log.h"

namespace sylar {
     
const char* LogLevel::ToString(LogLevel::Level level) {
    switch(level) {
#define XX(name) \ 
    case LogLevel::name: \
        return #name; \
        break;

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}

Logger::Logger(const std::string& name = "root") 
    :m_name(name) {
            
}

void Logger::addAppender(LogAppender::ptr appender) {
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin();
            it != m_appenders.end(); ++ it)
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        for (auto& i : m_appenders) {
            i->log(level, event);
        }
    }
}
    
void Logger::debug(LogEvent::ptr event) {
}
void Logger::info(LogEvent::ptr event) {       
}
void Logger::warn(LogEvent::ptr event) {       
}
    
void Logger::fatal(LogEvent::ptr event) {       
}
 
FileLogAppender::FileLogAppender(const std::string& filename) {
}

void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        m_filestream << m_formatter.format(event);
    }
}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}
 
void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter.format(event);
    }
}

LogFormatter::LogFormatter(const std::string& pattern) 
    :m_pattern(pattern) {
}

std::string LogFormatter::format(LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto& i : m_item) {
        i->format(ss, level, event);
    }
    return ss.str();
}

// %xxx %xxx{xxx} %%
void LogFormatter::init() {
    // str, format, type
    std::vector<std::tuple<std::string, std::string, int> > vec;
    std::string nstr;
    size_t last_pos = 0;
    for (size_t i = 0; i < m_pattern.size(); ++ i) {
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }
        if ((i + 1) < m_pattern.size()) {
            if (m_pattern[i + 1] == '%') {
                nstr.append(1, m_pattern[i]);
                continue;
            }
        }
        size_t n = i + 1;
        int fmt_status = 0; 
        size_t fmt_begin = 0;
        
        std::string str;
        std::string fmt;
        while (n < m_pattern.size()) {
            if (isspace(m_pattern[n])) {
               break;
            }
            if (fmt_status == 0) {
                if (m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    fmt_status = 1; // explain formatter
                    fmt_begin = n;
                    ++ n;
                    continue;
                }
            }
            if (fmt_status == 1) {
                if (m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 2;
                    ++ n;
                    continue;
                }
            }
        }

        if (fmt_status == 0) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, "", 0));
            }
            str = m_pattern.substr(i + 1, n - i - 1);
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n;
        } else if (fmt_status == 1) {
            std::cout << "pattern parse error: " << m_pattern << "-" << m_pattern.substr(i) << std::endl;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        } else if (fmt_status == 2) {
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n;
        }
    }
    
    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
     
    
class MessageFormatItem : public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};
class LevelFormatItem : public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) override {
        os << LogLevel::ToString(level);
    }
};

}
 
}
    