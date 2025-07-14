#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <memory>
#include <string>

namespace arbitrage {

class Logger {
public:
    static Logger& getInstance();
    
    // Initialize logging system
    bool initialize(const std::string& log_level = "info", 
                   const std::string& log_file = "logs/arbitrage_engine.log");
    
    // Get logger instance
    std::shared_ptr<spdlog::logger> getLogger() const;
    
    // Convenience methods
    template<typename... Args>
    void trace(const char* fmt, Args&&... args) {
        if (logger_) logger_->trace(fmt, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void debug(fmt::format_string<Args...> fmt, Args&&... args) {
        if (logger_) logger_->debug(fmt, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void info(fmt::format_string<Args...> fmt, Args&&... args) {
        if (logger_) logger_->info(fmt, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void warn(fmt::format_string<Args...> fmt, Args&&... args) {
        if (logger_) logger_->warn(fmt, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void error(fmt::format_string<Args...> fmt, Args&&... args) {
        if (logger_) logger_->error(fmt, std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    void critical(fmt::format_string<Args...> fmt, Args&&... args) {
        if (logger_) logger_->critical(fmt, std::forward<Args>(args)...);
    }
    
    // Performance logging
    template<typename... Args>
    void performance(fmt::format_string<Args...> fmt, Args&&... args) {
        if (performance_logger_) performance_logger_->info(fmt, std::forward<Args>(args)...);
    }
    
    // Market data logging
    template<typename... Args>
    void market_data(fmt::format_string<Args...> fmt, Args&&... args) {
        if (market_data_logger_) market_data_logger_->info(fmt, std::forward<Args>(args)...);
    }
    
    // Trade logging
    template<typename... Args>
    void trade(const char* fmt, Args&&... args) {
        if (trade_logger_) trade_logger_->info(fmt, std::forward<Args>(args)...);
    }
    
    // Risk logging
    template<typename... Args>
    void risk(const char* fmt, Args&&... args) {
        if (risk_logger_) risk_logger_->info(fmt, std::forward<Args>(args)...);
    }
    
    // Flush all loggers
    void flush();
    
private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    spdlog::level::level_enum stringToLogLevel(const std::string& level);
    void createDirectoryIfNotExists(const std::string& path);
    
    std::shared_ptr<spdlog::logger> logger_;
    std::shared_ptr<spdlog::logger> performance_logger_;
    std::shared_ptr<spdlog::logger> market_data_logger_;
    std::shared_ptr<spdlog::logger> trade_logger_;
    std::shared_ptr<spdlog::logger> risk_logger_;
    bool initialized_ = false;
};

// Global logger macros for convenience
#define LOG_TRACE(...) arbitrage::Logger::getInstance().trace(__VA_ARGS__)
#define LOG_DEBUG(...) arbitrage::Logger::getInstance().debug(__VA_ARGS__)
#define LOG_INFO(...) arbitrage::Logger::getInstance().info(__VA_ARGS__)
#define LOG_WARN(...) arbitrage::Logger::getInstance().warn(__VA_ARGS__)
#define LOG_ERROR(...) arbitrage::Logger::getInstance().error(__VA_ARGS__)
#define LOG_CRITICAL(...) arbitrage::Logger::getInstance().critical(__VA_ARGS__)

#define LOG_PERFORMANCE(...) arbitrage::Logger::getInstance().performance(__VA_ARGS__)
#define LOG_MARKET_DATA(...) arbitrage::Logger::getInstance().market_data(__VA_ARGS__)
#define LOG_TRADE(...) arbitrage::Logger::getInstance().trade(__VA_ARGS__)
#define LOG_RISK(...) arbitrage::Logger::getInstance().risk(__VA_ARGS__)

} // namespace arbitrage
