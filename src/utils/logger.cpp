#include "logger.hpp"
#include <filesystem>
#include <iostream>

namespace arbitrage {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::initialize(const std::string& log_level, const std::string& log_file) {
    try {
        // Create log directory if it doesn't exist
        std::filesystem::path log_path(log_file);
        if (log_path.has_parent_path()) {
            std::filesystem::create_directories(log_path.parent_path());
        }
        
        // Create sinks
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            log_file, 1048576 * 10, 5); // 10MB per file, 5 files max
        
        // Set patterns
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v");
        
        // Create main logger
        std::vector<spdlog::sink_ptr> sinks = {console_sink, file_sink};
        logger_ = std::make_shared<spdlog::logger>("arbitrage_engine", sinks.begin(), sinks.end());
        
        // Set log level
        auto level = stringToLogLevel(log_level);
        logger_->set_level(level);
        console_sink->set_level(level);
        file_sink->set_level(level);
        
        // Register with spdlog
        spdlog::register_logger(logger_);
        spdlog::set_default_logger(logger_);
        
        // Create specialized loggers
        auto performance_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/performance.log", 1048576 * 5, 3);
        performance_file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        performance_logger_ = std::make_shared<spdlog::logger>("performance", performance_file_sink);
        performance_logger_->set_level(spdlog::level::info);
        
        auto market_data_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/market_data.log", 1048576 * 20, 5);
        market_data_file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        market_data_logger_ = std::make_shared<spdlog::logger>("market_data", market_data_file_sink);
        market_data_logger_->set_level(spdlog::level::info);
        
        auto trade_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/trades.log", 1048576 * 10, 5);
        trade_file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        trade_logger_ = std::make_shared<spdlog::logger>("trades", trade_file_sink);
        trade_logger_->set_level(spdlog::level::info);
        
        auto risk_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/risk.log", 1048576 * 5, 3);
        risk_file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %v");
        risk_logger_ = std::make_shared<spdlog::logger>("risk", risk_file_sink);
        risk_logger_->set_level(spdlog::level::info);
        
        // Register specialized loggers
        spdlog::register_logger(performance_logger_);
        spdlog::register_logger(market_data_logger_);
        spdlog::register_logger(trade_logger_);
        spdlog::register_logger(risk_logger_);
        
        // Enable automatic flushing
        spdlog::flush_every(std::chrono::seconds(1));
        
        initialized_ = true;
        
        LOG_INFO("Logger initialized successfully");
        LOG_INFO("Log level: {}", log_level);
        LOG_INFO("Log file: {}", log_file);
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize logger: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() const {
    return logger_;
}

void Logger::flush() {
    if (logger_) logger_->flush();
    if (performance_logger_) performance_logger_->flush();
    if (market_data_logger_) market_data_logger_->flush();
    if (trade_logger_) trade_logger_->flush();
    if (risk_logger_) risk_logger_->flush();
}

spdlog::level::level_enum Logger::stringToLogLevel(const std::string& level) {
    if (level == "trace") return spdlog::level::trace;
    if (level == "debug") return spdlog::level::debug;
    if (level == "info") return spdlog::level::info;
    if (level == "warn") return spdlog::level::warn;
    if (level == "error") return spdlog::level::err;
    if (level == "critical") return spdlog::level::critical;
    return spdlog::level::info; // Default to info
}

void Logger::createDirectoryIfNotExists(const std::string& path) {
    std::filesystem::path dir_path(path);
    if (dir_path.has_parent_path()) {
        std::filesystem::create_directories(dir_path.parent_path());
    }
}

} // namespace arbitrage
