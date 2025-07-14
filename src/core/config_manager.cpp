#include "config_manager.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace arbitrage {

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadConfig(const std::string& config_file) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    
    try {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << config_file << std::endl;
            return false;
        }
        
        nlohmann::json json;
        file >> json;
        
        // Parse configuration sections
        parseSystemConfig(json["system"]);
        parseExchangeConfig(json["exchanges"]);
        parseInstrumentConfig(json["instruments"]);
        parseArbitrageConfig(json["arbitrage"]);
        
        config_file_path_ = config_file;
        config_loaded_ = true;
        
        if (!validateConfiguration()) {
            std::cerr << "Configuration validation failed" << std::endl;
            return false;
        }
        
        std::cout << "Configuration loaded successfully from: " << config_file << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
        return false;
    }
}

const SystemConfig& ConfigManager::getSystemConfig() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_loaded_) {
        throw std::runtime_error("Configuration not loaded");
    }
    return system_config_;
}

const ExchangeConfig& ConfigManager::getExchangeConfig(const std::string& exchange_name) const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_loaded_) {
        throw std::runtime_error("Configuration not loaded");
    }
    
    auto it = system_config_.exchanges.find(exchange_name);
    if (it == system_config_.exchanges.end()) {
        throw std::runtime_error("Exchange configuration not found: " + exchange_name);
    }
    
    return it->second;
}

const ArbitrageConfig& ConfigManager::getArbitrageConfig() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_loaded_) {
        throw std::runtime_error("Configuration not loaded");
    }
    return system_config_.arbitrage;
}

bool ConfigManager::isExchangeEnabled(const std::string& exchange_name) const {
    try {
        const auto& config = getExchangeConfig(exchange_name);
        return config.enabled;
    } catch (const std::exception&) {
        return false;
    }
}

std::vector<Instrument> ConfigManager::getEnabledInstruments() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_loaded_) {
        throw std::runtime_error("Configuration not loaded");
    }
    
    std::vector<Instrument> enabled_instruments;
    for (const auto& instrument : system_config_.instruments) {
        if (instrument.is_active) {
            enabled_instruments.push_back(instrument);
        }
    }
    
    return enabled_instruments;
}

std::vector<std::string> ConfigManager::getEnabledExchanges() const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    if (!config_loaded_) {
        throw std::runtime_error("Configuration not loaded");
    }
    
    std::vector<std::string> enabled_exchanges;
    for (const auto& [exchange_name, config] : system_config_.exchanges) {
        if (config.enabled) {
            enabled_exchanges.push_back(exchange_name);
        }
    }
    
    return enabled_exchanges;
}

void ConfigManager::updateArbitrageConfig(const ArbitrageConfig& config) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    system_config_.arbitrage = config;
}

void ConfigManager::enableExchange(const std::string& exchange_name, bool enabled) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    auto it = system_config_.exchanges.find(exchange_name);
    if (it != system_config_.exchanges.end()) {
        it->second.enabled = enabled;
    }
}

bool ConfigManager::validateConfiguration() const {
    // Validate system configuration
    if (system_config_.log_level.empty()) {
        std::cerr << "Log level not specified" << std::endl;
        return false;
    }
    
    if (system_config_.thread_pool_size <= 0) {
        std::cerr << "Invalid thread pool size" << std::endl;
        return false;
    }
    
    // Validate exchange configurations
    bool has_enabled_exchange = false;
    for (const auto& [exchange_name, config] : system_config_.exchanges) {
        if (config.enabled) {
            has_enabled_exchange = true;
            if (config.websocket_url.empty()) {
                std::cerr << "WebSocket URL not specified for exchange: " << exchange_name << std::endl;
                return false;
            }
            if (config.connection_timeout_ms <= 0) {
                std::cerr << "Invalid connection timeout for exchange: " << exchange_name << std::endl;
                return false;
            }
        }
    }
    
    if (!has_enabled_exchange) {
        std::cerr << "No exchanges enabled" << std::endl;
        return false;
    }
    
    // Validate instrument configuration
    bool has_enabled_instrument = false;
    for (const auto& instrument : system_config_.instruments) {
        if (instrument.is_active) {
            has_enabled_instrument = true;
            if (instrument.symbol.empty()) {
                std::cerr << "Symbol not specified for instrument" << std::endl;
                return false;
            }
            if (instrument.tick_size <= 0) {
                std::cerr << "Invalid tick size for instrument: " << instrument.symbol << std::endl;
                return false;
            }
        }
    }
    
    if (!has_enabled_instrument) {
        std::cerr << "No instruments enabled" << std::endl;
        return false;
    }
    
    // Validate arbitrage configuration
    if (system_config_.arbitrage.min_profit_threshold <= 0) {
        std::cerr << "Invalid minimum profit threshold" << std::endl;
        return false;
    }
    
    if (system_config_.arbitrage.max_position_size <= 0) {
        std::cerr << "Invalid maximum position size" << std::endl;
        return false;
    }
    
    return true;
}

void ConfigManager::parseSystemConfig(const nlohmann::json& json) {
    system_config_.log_level = json.value("log_level", "info");
    system_config_.log_file = json.value("log_file", "logs/arbitrage_engine.log");
    system_config_.performance_monitoring = json.value("performance_monitoring", true);
    system_config_.memory_pool_size = json.value("memory_pool_size", 1073741824ULL);
    system_config_.thread_pool_size = json.value("thread_pool_size", 8);
}

void ConfigManager::parseExchangeConfig(const nlohmann::json& json) {
    for (const auto& [exchange_name, exchange_json] : json.items()) {
        ExchangeConfig config;
        config.enabled = exchange_json.value("enabled", false);
        config.websocket_url = exchange_json.value("websocket_url", "");
        config.rest_url = exchange_json.value("rest_url", "");
        config.connection_timeout_ms = exchange_json.value("connection_timeout", 10000);
        config.reconnect_interval_ms = exchange_json.value("reconnect_interval", 5000);
        config.max_reconnect_attempts = exchange_json.value("max_reconnect_attempts", 10);
        
        if (exchange_json.contains("rate_limit")) {
            const auto& rate_limit = exchange_json["rate_limit"];
            config.rate_limit.requests_per_second = rate_limit.value("requests_per_second", 20);
            config.rate_limit.burst_size = rate_limit.value("burst_size", 100);
        }
        
        system_config_.exchanges[exchange_name] = config;
    }
}

void ConfigManager::parseInstrumentConfig(const nlohmann::json& json) {
    // Parse spot pairs
    if (json.contains("spot_pairs")) {
        for (const auto& spot_json : json["spot_pairs"]) {
            Instrument instrument;
            instrument.symbol = spot_json.value("symbol", "");
            instrument.base_asset = spot_json.value("base", "");
            instrument.quote_asset = spot_json.value("quote", "");
            instrument.type = InstrumentType::SPOT;
            instrument.is_active = spot_json.value("enabled", false);
            instrument.min_notional = spot_json.value("min_notional", 10.0);
            instrument.tick_size = spot_json.value("tick_size", 0.01);
            instrument.contract_size = 1.0;
            
            // Generate instrument ID
            instrument.id = instrument.symbol + "_SPOT";
            
            system_config_.instruments.push_back(instrument);
        }
    }
    
    // Parse derivatives
    if (json.contains("derivatives")) {
        for (const auto& deriv_json : json["derivatives"]) {
            Instrument instrument;
            instrument.symbol = deriv_json.value("symbol", "");
            instrument.base_asset = deriv_json.value("underlying", "");
            instrument.quote_asset = deriv_json.value("quote", "");
            instrument.type = stringToInstrumentType(deriv_json.value("type", ""));
            instrument.is_active = deriv_json.value("enabled", false);
            instrument.contract_size = deriv_json.value("contract_size", 1.0);
            instrument.tick_size = deriv_json.value("tick_size", 0.01);
            instrument.min_notional = 10.0;
            
            // Generate instrument ID
            instrument.id = instrument.symbol + "_" + instrumentTypeToString(instrument.type);
            
            system_config_.instruments.push_back(instrument);
        }
    }
}

void ConfigManager::parseArbitrageConfig(const nlohmann::json& json) {
    const auto& detection = json["detection"];
    system_config_.arbitrage.min_profit_threshold = detection.value("min_profit_threshold", 0.001);
    system_config_.arbitrage.max_latency_ms = detection.value("max_latency_ms", 10);
    system_config_.arbitrage.signal_strength_threshold = detection.value("signal_strength_threshold", 0.7);
    system_config_.arbitrage.confidence_threshold = detection.value("confidence_threshold", 0.95);
    
    const auto& risk_mgmt = json["risk_management"];
    system_config_.arbitrage.max_position_size = risk_mgmt.value("max_position_size", 10000.0);
    system_config_.arbitrage.max_portfolio_exposure = risk_mgmt.value("max_portfolio_exposure", 100000.0);
    system_config_.arbitrage.max_leverage = risk_mgmt.value("max_leverage", 10.0);
    system_config_.arbitrage.stop_loss_percentage = risk_mgmt.value("stop_loss_percentage", 0.02);
    system_config_.arbitrage.take_profit_percentage = risk_mgmt.value("take_profit_percentage", 0.01);
}

} // namespace arbitrage
