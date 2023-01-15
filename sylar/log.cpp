#include "log.h"

namespace sylar {

    Logger::Logger(const std::string& name = "root") 
        :m_name(name) {
            
        }
    void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
        
    }
    
    void Logger::debug(LogEvent::ptr event) {
    }
    void Logger::info(LogEvent::ptr event) {
        
    }
    void Logger::warn(LogEvent::ptr event) {
        
    }
    void Logger::error(LogEvent::ptr event) {
        
    }
    void Logger::fatal(LogEvent::ptr event) {
        
    }
}