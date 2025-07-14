#include <gtest/gtest.h>
#include "config_manager.hpp"
#include "logger.hpp"
#include "performance_monitor.hpp"
#include <fstream>
#include <filesystem>

namespace arbitrage {

class ConfigManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary config file for testing
        test_config_file_ = "/tmp/test_config.json";
        createTestConfig();
    }
    
    void TearDown() override {
        // Clean up test file
        if (std::filesystem::exists(test_config_file_)) {
            std::filesystem::remove(test_config_file_);
        }
    }
    
    void createTestConfig() {
        const std::string config_content = R"({
            "system": {
                "log_level": "debug",
                "log_file": "test.log",
                "performance_monitoring": true,
                "memory_pool_size": 1073741824,
                "thread_pool_size": 4
            },
            "exchanges": {
                "okx": {
                    "enabled": true,
                    "websocket_url": "wss://ws.okx.com:8443/ws/v5/public",
                    "rest_url": "https://www.okx.com/api/v5",
                    "connection_timeout": 5000,
                    "reconnect_interval": 2000,
                    "max_reconnect_attempts": 5,
                    "rate_limit": {
                        "requests_per_second": 10,
                        "burst_size": 50
                    }
                },
                "binance": {
                    "enabled": false,
                    "websocket_url": "wss://stream.binance.com:443/ws/",
                    "rest_url": "https://api.binance.com/api/v3",
                    "connection_timeout": 10000,
                    "reconnect_interval": 5000,
                    "max_reconnect_attempts": 10,
                    "rate_limit": {
                        "requests_per_second": 20,
                        "burst_size": 100
                    }
                }
            },
            "instruments": {
                "spot_pairs": [
                    {
                        "symbol": "BTC/USDT",
                        "base": "BTC",
                        "quote": "USDT",
                        "enabled": true,
                        "min_notional": 10.0,
                        "tick_size": 0.01
                    }
                ],
                "derivatives": [
                    {
                        "symbol": "BTC-PERPETUAL",
                        "underlying": "BTC",
                        "quote": "USDT",
                        "type": "perpetual_swap",
                        "enabled": true,
                        "contract_size": 1.0,
                        "tick_size": 0.1
                    }
                ]
            },
            "arbitrage": {
                "detection": {
                    "min_profit_threshold": 0.002,
                    "max_latency_ms": 5,
                    "signal_strength_threshold": 0.8,
                    "confidence_threshold": 0.9
                },
                "risk_management": {
                    "max_position_size": 5000.0,
                    "max_portfolio_exposure": 50000.0,
                    "max_leverage": 5.0,
                    "stop_loss_percentage": 0.01,
                    "take_profit_percentage": 0.005
                }
            }
        })";
        
        std::ofstream file(test_config_file_);
        file << config_content;
        file.close();
    }
    
    std::string test_config_file_;
};

TEST_F(ConfigManagerTest, LoadConfiguration) {
    auto& config_manager = ConfigManager::getInstance();
    
    EXPECT_TRUE(config_manager.loadConfig(test_config_file_));
    
    const auto& system_config = config_manager.getSystemConfig();
    EXPECT_EQ(system_config.log_level, "debug");
    EXPECT_EQ(system_config.thread_pool_size, 4);
    EXPECT_EQ(system_config.memory_pool_size, 1073741824ULL);
    EXPECT_TRUE(system_config.performance_monitoring);
}

TEST_F(ConfigManagerTest, ExchangeConfiguration) {
    auto& config_manager = ConfigManager::getInstance();
    EXPECT_TRUE(config_manager.loadConfig(test_config_file_));
    
    // Test enabled exchange
    EXPECT_TRUE(config_manager.isExchangeEnabled("okx"));
    const auto& okx_config = config_manager.getExchangeConfig("okx");
    EXPECT_TRUE(okx_config.enabled);
    EXPECT_EQ(okx_config.websocket_url, "wss://ws.okx.com:8443/ws/v5/public");
    EXPECT_EQ(okx_config.connection_timeout_ms, 5000);
    
    // Test disabled exchange
    EXPECT_FALSE(config_manager.isExchangeEnabled("binance"));
    const auto& binance_config = config_manager.getExchangeConfig("binance");
    EXPECT_FALSE(binance_config.enabled);
    
    // Test enabled exchanges list
    auto enabled_exchanges = config_manager.getEnabledExchanges();
    EXPECT_EQ(enabled_exchanges.size(), 1);
    EXPECT_EQ(enabled_exchanges[0], "okx");
}

TEST_F(ConfigManagerTest, InstrumentConfiguration) {
    auto& config_manager = ConfigManager::getInstance();
    EXPECT_TRUE(config_manager.loadConfig(test_config_file_));
    
    auto enabled_instruments = config_manager.getEnabledInstruments();
    EXPECT_EQ(enabled_instruments.size(), 2);
    
    // Check spot instrument
    bool found_spot = false;
    for (const auto& instrument : enabled_instruments) {
        if (instrument.symbol == "BTC/USDT" && instrument.type == InstrumentType::SPOT) {
            found_spot = true;
            EXPECT_EQ(instrument.base_asset, "BTC");
            EXPECT_EQ(instrument.quote_asset, "USDT");
            EXPECT_EQ(instrument.tick_size, 0.01);
            EXPECT_EQ(instrument.min_notional, 10.0);
            break;
        }
    }
    EXPECT_TRUE(found_spot);
    
    // Check derivative instrument
    bool found_derivative = false;
    for (const auto& instrument : enabled_instruments) {
        if (instrument.symbol == "BTC-PERPETUAL" && instrument.type == InstrumentType::PERPETUAL_SWAP) {
            found_derivative = true;
            EXPECT_EQ(instrument.base_asset, "BTC");
            EXPECT_EQ(instrument.quote_asset, "USDT");
            EXPECT_EQ(instrument.tick_size, 0.1);
            EXPECT_EQ(instrument.contract_size, 1.0);
            break;
        }
    }
    EXPECT_TRUE(found_derivative);
}

TEST_F(ConfigManagerTest, ArbitrageConfiguration) {
    auto& config_manager = ConfigManager::getInstance();
    EXPECT_TRUE(config_manager.loadConfig(test_config_file_));
    
    const auto& arbitrage_config = config_manager.getArbitrageConfig();
    EXPECT_EQ(arbitrage_config.min_profit_threshold, 0.002);
    EXPECT_EQ(arbitrage_config.max_latency_ms, 5);
    EXPECT_EQ(arbitrage_config.signal_strength_threshold, 0.8);
    EXPECT_EQ(arbitrage_config.confidence_threshold, 0.9);
    EXPECT_EQ(arbitrage_config.max_position_size, 5000.0);
    EXPECT_EQ(arbitrage_config.max_portfolio_exposure, 50000.0);
    EXPECT_EQ(arbitrage_config.max_leverage, 5.0);
    EXPECT_EQ(arbitrage_config.stop_loss_percentage, 0.01);
    EXPECT_EQ(arbitrage_config.take_profit_percentage, 0.005);
}

class PerformanceMonitorTest : public ::testing::Test {
protected:
    void SetUp() override {
        perf_monitor_ = &PerformanceMonitor::getInstance();
        perf_monitor_->initialize(100);  // 100ms interval for faster testing
    }
    
    void TearDown() override {
        perf_monitor_->stop();
        perf_monitor_->resetMetrics();
    }
    
    PerformanceMonitor* perf_monitor_;
};

TEST_F(PerformanceMonitorTest, BasicMetrics) {
    EXPECT_EQ(perf_monitor_->getMessagesProcessed(), 0);
    EXPECT_EQ(perf_monitor_->getOpportunitiesDetected(), 0);
    EXPECT_EQ(perf_monitor_->getTradesExecuted(), 0);
    
    perf_monitor_->recordMessageProcessed();
    perf_monitor_->recordOpportunityDetected();
    perf_monitor_->recordTradeExecuted();
    
    EXPECT_EQ(perf_monitor_->getMessagesProcessed(), 1);
    EXPECT_EQ(perf_monitor_->getOpportunitiesDetected(), 1);
    EXPECT_EQ(perf_monitor_->getTradesExecuted(), 1);
}

TEST_F(PerformanceMonitorTest, LatencyTracking) {
    perf_monitor_->recordLatency(10.0);
    perf_monitor_->recordLatency(20.0);
    perf_monitor_->recordLatency(30.0);
    
    EXPECT_EQ(perf_monitor_->getAverageLatency(), 20.0);
    EXPECT_EQ(perf_monitor_->getMaxLatency(), 30.0);
    
    perf_monitor_->recordLatency(5.0);
    EXPECT_EQ(perf_monitor_->getAverageLatency(), 16.25);  // (10+20+30+5)/4
    EXPECT_EQ(perf_monitor_->getMaxLatency(), 30.0);
}

TEST_F(PerformanceMonitorTest, ThresholdChecking) {
    perf_monitor_->recordLatency(5.0);
    perf_monitor_->recordMemoryUsage(100.0);
    
    EXPECT_TRUE(perf_monitor_->isLatencyWithinThreshold(10.0));
    EXPECT_FALSE(perf_monitor_->isLatencyWithinThreshold(3.0));
    
    EXPECT_TRUE(perf_monitor_->isMemoryWithinThreshold(200.0));
    EXPECT_FALSE(perf_monitor_->isMemoryWithinThreshold(50.0));
}

TEST_F(PerformanceMonitorTest, ResetMetrics) {
    perf_monitor_->recordMessageProcessed();
    perf_monitor_->recordOpportunityDetected();
    perf_monitor_->recordLatency(15.0);
    
    EXPECT_GT(perf_monitor_->getMessagesProcessed(), 0);
    EXPECT_GT(perf_monitor_->getOpportunitiesDetected(), 0);
    EXPECT_GT(perf_monitor_->getAverageLatency(), 0);
    
    perf_monitor_->resetMetrics();
    
    EXPECT_EQ(perf_monitor_->getMessagesProcessed(), 0);
    EXPECT_EQ(perf_monitor_->getOpportunitiesDetected(), 0);
    EXPECT_EQ(perf_monitor_->getAverageLatency(), 0);
}

// Test type utilities
TEST(TypeUtilsTest, ExchangeConversion) {
    EXPECT_EQ(exchangeToString(Exchange::OKX), "OKX");
    EXPECT_EQ(exchangeToString(Exchange::BINANCE), "BINANCE");
    EXPECT_EQ(exchangeToString(Exchange::BYBIT), "BYBIT");
    EXPECT_EQ(exchangeToString(Exchange::UNKNOWN), "UNKNOWN");
    
    EXPECT_EQ(stringToExchange("OKX"), Exchange::OKX);
    EXPECT_EQ(stringToExchange("BINANCE"), Exchange::BINANCE);
    EXPECT_EQ(stringToExchange("BYBIT"), Exchange::BYBIT);
    EXPECT_EQ(stringToExchange("INVALID"), Exchange::UNKNOWN);
}

TEST(TypeUtilsTest, InstrumentTypeConversion) {
    EXPECT_EQ(instrumentTypeToString(InstrumentType::SPOT), "SPOT");
    EXPECT_EQ(instrumentTypeToString(InstrumentType::PERPETUAL_SWAP), "PERPETUAL_SWAP");
    EXPECT_EQ(instrumentTypeToString(InstrumentType::FUTURES), "FUTURES");
    EXPECT_EQ(instrumentTypeToString(InstrumentType::OPTION), "OPTION");
    EXPECT_EQ(instrumentTypeToString(InstrumentType::UNKNOWN), "UNKNOWN");
    
    EXPECT_EQ(stringToInstrumentType("SPOT"), InstrumentType::SPOT);
    EXPECT_EQ(stringToInstrumentType("PERPETUAL_SWAP"), InstrumentType::PERPETUAL_SWAP);
    EXPECT_EQ(stringToInstrumentType("FUTURES"), InstrumentType::FUTURES);
    EXPECT_EQ(stringToInstrumentType("OPTION"), InstrumentType::OPTION);
    EXPECT_EQ(stringToInstrumentType("INVALID"), InstrumentType::UNKNOWN);
}

TEST(TypeUtilsTest, OrderBookOperations) {
    OrderBook order_book;
    order_book.bids.push_back({100.0, 10.0, getCurrentTimestamp()});
    order_book.bids.push_back({99.0, 15.0, getCurrentTimestamp()});
    order_book.asks.push_back({101.0, 8.0, getCurrentTimestamp()});
    order_book.asks.push_back({102.0, 12.0, getCurrentTimestamp()});
    
    EXPECT_EQ(order_book.getBestBid(), 100.0);
    EXPECT_EQ(order_book.getBestAsk(), 101.0);
    EXPECT_EQ(order_book.getMidPrice(), 100.5);
    EXPECT_EQ(order_book.getSpread(), 1.0);
}

} // namespace arbitrage
