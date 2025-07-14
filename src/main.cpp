#include "config_manager.hpp"
#include "logger.hpp"
#include "performance_monitor.hpp"
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <unistd.h>

namespace arbitrage {

class ArbitrageEngine {
public:
    ArbitrageEngine() = default;
    ~ArbitrageEngine() = default;
    
    bool initialize(const std::string& config_file);
    void run();
    void shutdown();
    
private:
    bool setupSignalHandlers();
    void printSystemInfo();
    void printConfiguration();
    
    std::atomic<bool> running_{false};
    std::atomic<bool> shutdown_requested_{false};
};

// Global instance for signal handling
std::atomic<ArbitrageEngine*> g_engine_instance{nullptr};

// Signal handler
void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << std::endl;
    
    ArbitrageEngine* engine = g_engine_instance.load();
    if (engine) {
        engine->shutdown();
    }
}

bool ArbitrageEngine::initialize(const std::string& config_file) {
    try {
        // Initialize configuration manager
        auto& config_manager = ConfigManager::getInstance();
        if (!config_manager.loadConfig(config_file)) {
            std::cerr << "Failed to load configuration from: " << config_file << std::endl;
            return false;
        }
        
        // Get system configuration
        const auto& system_config = config_manager.getSystemConfig();
        
        // Initialize logger
        auto& logger = Logger::getInstance();
        if (!logger.initialize(system_config.log_level, system_config.log_file)) {
            std::cerr << "Failed to initialize logger" << std::endl;
            return false;
        }
        
        LOG_INFO("=== Synthetic Arbitrage Detection Engine ===");
        LOG_INFO("Version: 1.0.0");
        LOG_INFO("Build: {}", __DATE__ " " __TIME__);
        
        // Initialize performance monitor
        auto& perf_monitor = PerformanceMonitor::getInstance();
        if (!perf_monitor.initialize(1000)) {
            LOG_ERROR("Failed to initialize performance monitor");
            return false;
        }
        
        // Setup performance alerts
        perf_monitor.setLatencyAlertCallback(
            [](const std::string& type, const std::string& message) {
                LOG_WARN("{}: {}", type, message);
            }, 50.0  // 50ms threshold
        );
        
        perf_monitor.setMemoryAlertCallback(
            [](const std::string& type, const std::string& message) {
                LOG_WARN("{}: {}", type, message);
            }, 2048.0  // 2GB threshold
        );
        
        perf_monitor.setCpuAlertCallback(
            [](const std::string& type, const std::string& message) {
                LOG_WARN("{}: {}", type, message);
            }, 80.0  // 80% threshold
        );
        
        // Setup signal handlers
        if (!setupSignalHandlers()) {
            LOG_ERROR("Failed to setup signal handlers");
            return false;
        }
        
        // Print system information
        printSystemInfo();
        printConfiguration();
        
        LOG_INFO("Engine initialization completed successfully");
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

void ArbitrageEngine::run() {
    LOG_INFO("Starting Synthetic Arbitrage Detection Engine...");
    
    running_ = true;
    g_engine_instance = this;
    
    // Start performance monitoring
    auto& perf_monitor = PerformanceMonitor::getInstance();
    perf_monitor.start();
    
    // TODO: Phase 2 - Initialize exchange connections
    // TODO: Phase 3 - Initialize pricing models
    // TODO: Phase 4 - Initialize arbitrage detection
    // TODO: Phase 5 - Initialize risk management
    
    LOG_INFO("Engine is running. Press Ctrl+C to stop.");
    
    // Main loop - placeholder for now
    while (running_ && !shutdown_requested_) {
        try {
            // TODO: Process market data
            // TODO: Detect arbitrage opportunities
            // TODO: Execute trades
            // TODO: Monitor risk
            
            // For now, just sleep to demonstrate the framework
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            // Simulate some activity for testing
            static int counter = 0;
            if (++counter % 10 == 0) {
                perf_monitor.recordMessageProcessed();
                perf_monitor.recordLatency(5.0 + (counter % 10));
                
                if (counter % 100 == 0) {
                    perf_monitor.recordOpportunityDetected();
                    LOG_INFO("Simulated opportunity detected (counter: {})", counter);
                }
            }
            
        } catch (const std::exception& e) {
            LOG_ERROR("Exception in main loop: {}", e.what());
        }
    }
    
    LOG_INFO("Engine main loop stopped");
}

void ArbitrageEngine::shutdown() {
    LOG_INFO("Shutting down Synthetic Arbitrage Detection Engine...");
    
    shutdown_requested_ = true;
    running_ = false;
    
    // Stop performance monitoring
    auto& perf_monitor = PerformanceMonitor::getInstance();
    perf_monitor.stop();
    
    // Print final statistics
    auto metrics = perf_monitor.getMetrics();
    LOG_INFO("Final Statistics:");
    LOG_INFO("  Messages Processed: {}", metrics.messages_processed);
    LOG_INFO("  Opportunities Detected: {}", metrics.opportunities_detected);
    LOG_INFO("  Trades Executed: {}", metrics.trades_executed);
    LOG_INFO("  Average Latency: {:.2f}ms", metrics.average_latency_ms);
    LOG_INFO("  Max Latency: {:.2f}ms", metrics.max_latency_ms);
    LOG_INFO("  Memory Usage: {:.2f}MB", metrics.memory_usage_mb);
    LOG_INFO("  CPU Usage: {:.2f}%", metrics.cpu_usage_percentage);
    
    // TODO: Cleanup exchange connections
    // TODO: Save state if needed
    
    LOG_INFO("Engine shutdown completed");
    
    // Flush all loggers
    Logger::getInstance().flush();
}

bool ArbitrageEngine::setupSignalHandlers() {
    try {
        signal(SIGINT, signalHandler);   // Ctrl+C
        signal(SIGTERM, signalHandler);  // Termination request
        signal(SIGQUIT, signalHandler);  // Quit signal
        
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to setup signal handlers: {}", e.what());
        return false;
    }
}

void ArbitrageEngine::printSystemInfo() {
    LOG_INFO("System Information:");
    LOG_INFO("  CPU Cores: {}", std::thread::hardware_concurrency());
    LOG_INFO("  Page Size: {}", sysconf(_SC_PAGESIZE));
    LOG_INFO("  PID: {}", getpid());
    
    // Get current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        LOG_INFO("  Working Directory: {}", cwd);
    }
}

void ArbitrageEngine::printConfiguration() {
    try {
        const auto& config = ConfigManager::getInstance().getSystemConfig();
        
        LOG_INFO("Configuration:");
        LOG_INFO("  Log Level: {}", config.log_level);
        LOG_INFO("  Thread Pool Size: {}", config.thread_pool_size);
        LOG_INFO("  Memory Pool Size: {:.2f}MB", config.memory_pool_size / 1024.0 / 1024.0);
        LOG_INFO("  Performance Monitoring: {}", config.performance_monitoring ? "enabled" : "disabled");
        
        // Print enabled exchanges
        auto enabled_exchanges = ConfigManager::getInstance().getEnabledExchanges();
        LOG_INFO("  Enabled Exchanges: {}", enabled_exchanges.size());
        for (const auto& exchange : enabled_exchanges) {
            LOG_INFO("    - {}", exchange);
        }
        
        // Print enabled instruments
        auto enabled_instruments = ConfigManager::getInstance().getEnabledInstruments();
        LOG_INFO("  Enabled Instruments: {}", enabled_instruments.size());
        for (const auto& instrument : enabled_instruments) {
            LOG_INFO("    - {} ({}, {})", instrument.symbol, 
                    instrumentTypeToString(instrument.type),
                    exchangeToString(instrument.exchange));
        }
        
        // Print arbitrage configuration
        LOG_INFO("  Min Profit Threshold: {:.4f}%", config.arbitrage.min_profit_threshold * 100);
        LOG_INFO("  Max Position Size: ${:.2f}", config.arbitrage.max_position_size);
        LOG_INFO("  Max Latency: {}ms", config.arbitrage.max_latency_ms);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to print configuration: {}", e.what());
    }
}

} // namespace arbitrage

int main(int argc, char* argv[]) {
    try {
        // Default configuration file
        std::string config_file = "config/engine_config.json";
        
        // Parse command line arguments
        if (argc > 1) {
            config_file = argv[1];
        }
        
        std::cout << "Synthetic Arbitrage Detection Engine v1.0.0" << std::endl;
        std::cout << "Loading configuration from: " << config_file << std::endl;
        
        // Create and initialize engine
        arbitrage::ArbitrageEngine engine;
        if (!engine.initialize(config_file)) {
            std::cerr << "Failed to initialize engine" << std::endl;
            return 1;
        }
        
        // Run the engine
        engine.run();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}
