#pragma once

#include "types.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <mutex>

namespace arbitrage {

class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    // Load configuration from file
    bool loadConfig(const std::string& config_file);
    
    // Get configuration sections
    const SystemConfig& getSystemConfig() const;
    const ExchangeConfig& getExchangeConfig(const std::string& exchange_name) const;
    const ArbitrageConfig& getArbitrageConfig() const;
    
    // Get specific configuration values
    bool isExchangeEnabled(const std::string& exchange_name) const;
    std::vector<Instrument> getEnabledInstruments() const;
    std::vector<std::string> getEnabledExchanges() const;
    
    // Update configuration at runtime
    void updateArbitrageConfig(const ArbitrageConfig& config);
    void enableExchange(const std::string& exchange_name, bool enabled);
    
    // Validation
    bool validateConfiguration() const;
    
private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    // Helper methods
    void parseSystemConfig(const nlohmann::json& json);
    void parseExchangeConfig(const nlohmann::json& json);
    void parseInstrumentConfig(const nlohmann::json& json);
    void parseArbitrageConfig(const nlohmann::json& json);
    
    mutable std::mutex config_mutex_;
    SystemConfig system_config_;
    bool config_loaded_ = false;
    std::string config_file_path_;
};

} // namespace arbitrage
