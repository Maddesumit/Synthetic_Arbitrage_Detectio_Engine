#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <atomic>
#include <memory>

namespace arbitrage {

// Forward declarations
class MarketData;
class ArbitrageOpportunity;
class RiskMetrics;

// Type aliases for better readability
using Timestamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Price = double;
using Volume = double;
using OrderId = std::string;
using InstrumentId = std::string;
using ExchangeId = std::string;

// Enumerations
enum class Exchange {
    OKX,
    BINANCE,
    BYBIT,
    UNKNOWN
};

enum class InstrumentType {
    SPOT,
    PERPETUAL_SWAP,
    FUTURES,
    OPTION,
    UNKNOWN
};

enum class OrderSide {
    BUY,
    SELL,
    UNKNOWN
};

enum class ArbitrageType {
    REAL_VS_SYNTHETIC_SPOT,
    REAL_VS_SYNTHETIC_DERIVATIVE,
    CROSS_SYNTHETIC,
    FUNDING_RATE_ARBITRAGE,
    BASIS_SPREAD_ARBITRAGE,
    UNKNOWN
};

enum class OrderBookLevel {
    L1,  // Best bid/ask
    L2,  // Full order book
    L3   // Order by order
};

// Core data structures
struct OrderBookEntry {
    Price price;
    Volume volume;
    Timestamp timestamp;
    
    OrderBookEntry() : price(0.0), volume(0.0) {}
    OrderBookEntry(Price p, Volume v, Timestamp t) : price(p), volume(v), timestamp(t) {}
};

struct OrderBook {
    std::vector<OrderBookEntry> bids;
    std::vector<OrderBookEntry> asks;
    Timestamp timestamp;
    InstrumentId instrument_id;
    ExchangeId exchange_id;
    
    // Helper methods
    Price getBestBid() const { return bids.empty() ? 0.0 : bids[0].price; }
    Price getBestAsk() const { return asks.empty() ? 0.0 : asks[0].price; }
    Price getMidPrice() const { return (getBestBid() + getBestAsk()) / 2.0; }
    Price getSpread() const { return getBestAsk() - getBestBid(); }
};

struct Trade {
    OrderId trade_id;
    InstrumentId instrument_id;
    ExchangeId exchange_id;
    Price price;
    Volume volume;
    OrderSide side;
    Timestamp timestamp;
    
    Trade() : price(0.0), volume(0.0), side(OrderSide::UNKNOWN) {}
};

struct Ticker {
    InstrumentId instrument_id;
    ExchangeId exchange_id;
    Price last_price;
    Price bid_price;
    Price ask_price;
    Volume volume_24h;
    Price price_change_24h;
    Price price_change_percentage_24h;
    Timestamp timestamp;
    
    Ticker() : last_price(0.0), bid_price(0.0), ask_price(0.0), 
               volume_24h(0.0), price_change_24h(0.0), price_change_percentage_24h(0.0) {}
};

struct FundingRate {
    InstrumentId instrument_id;
    ExchangeId exchange_id;
    Price current_rate;
    Price predicted_rate;
    Timestamp funding_time;
    Timestamp next_funding_time;
    Timestamp timestamp;
    
    FundingRate() : current_rate(0.0), predicted_rate(0.0) {}
};

struct Instrument {
    InstrumentId id;
    std::string symbol;
    std::string base_asset;
    std::string quote_asset;
    InstrumentType type;
    Exchange exchange;
    Price tick_size;
    Price min_notional;
    Price contract_size;
    Timestamp expiry_time;  // For futures and options
    bool is_active;
    
    Instrument() : type(InstrumentType::UNKNOWN), exchange(Exchange::UNKNOWN),
                   tick_size(0.0), min_notional(0.0), contract_size(1.0), is_active(false) {}
};

struct SyntheticPrice {
    InstrumentId synthetic_instrument_id;
    Price calculated_price;
    Price fair_value;
    Price basis_spread;
    std::vector<InstrumentId> component_instruments;
    std::vector<Price> component_weights;
    Timestamp calculation_time;
    double confidence_score;
    
    SyntheticPrice() : calculated_price(0.0), fair_value(0.0), basis_spread(0.0), confidence_score(0.0) {}
};

struct ArbitrageOpportunity {
    std::string opportunity_id;
    ArbitrageType type;
    std::vector<InstrumentId> leg_instruments;
    std::vector<Exchange> leg_exchanges;
    std::vector<OrderSide> leg_sides;
    std::vector<Price> leg_prices;
    std::vector<Volume> leg_volumes;
    Price expected_profit;
    Price expected_profit_percentage;
    double risk_score;
    double confidence_score;
    Timestamp detection_time;
    Timestamp expiry_time;
    bool is_active;
    
    ArbitrageOpportunity() : expected_profit(0.0), expected_profit_percentage(0.0),
                            risk_score(0.0), confidence_score(0.0), is_active(false) {}
};

struct RiskMetrics {
    double value_at_risk;
    double maximum_drawdown;
    double sharpe_ratio;
    double correlation_risk;
    double liquidity_risk;
    double funding_rate_risk;
    double basis_risk;
    Timestamp calculation_time;
    
    RiskMetrics() : value_at_risk(0.0), maximum_drawdown(0.0), sharpe_ratio(0.0),
                   correlation_risk(0.0), liquidity_risk(0.0), funding_rate_risk(0.0), basis_risk(0.0) {}
};

struct PerformanceMetrics {
    uint64_t messages_processed{0};
    uint64_t opportunities_detected{0};
    uint64_t trades_executed{0};
    double average_latency_ms{0.0};
    double max_latency_ms{0.0};
    double memory_usage_mb{0.0};
    double cpu_usage_percentage{0.0};
    Timestamp last_update;
    
    void reset() {
        messages_processed = 0;
        opportunities_detected = 0;
        trades_executed = 0;
        average_latency_ms = 0.0;
        max_latency_ms = 0.0;
        memory_usage_mb = 0.0;
        cpu_usage_percentage = 0.0;
    }
};

struct AtomicPerformanceMetrics {
    std::atomic<uint64_t> messages_processed{0};
    std::atomic<uint64_t> opportunities_detected{0};
    std::atomic<uint64_t> trades_executed{0};
    std::atomic<double> average_latency_ms{0.0};
    std::atomic<double> max_latency_ms{0.0};
    std::atomic<double> memory_usage_mb{0.0};
    std::atomic<double> cpu_usage_percentage{0.0};
    Timestamp last_update;
    
    void reset() {
        messages_processed = 0;
        opportunities_detected = 0;
        trades_executed = 0;
        average_latency_ms = 0.0;
        max_latency_ms = 0.0;
        memory_usage_mb = 0.0;
        cpu_usage_percentage = 0.0;
    }
};

// Configuration structures
struct ExchangeConfig {
    bool enabled;
    std::string websocket_url;
    std::string rest_url;
    int connection_timeout_ms;
    int reconnect_interval_ms;
    int max_reconnect_attempts;
    struct {
        int requests_per_second;
        int burst_size;
    } rate_limit;
};

struct ArbitrageConfig {
    double min_profit_threshold;
    int max_latency_ms;
    double signal_strength_threshold;
    double confidence_threshold;
    double max_position_size;
    double max_portfolio_exposure;
    double max_leverage;
    double stop_loss_percentage;
    double take_profit_percentage;
};

struct SystemConfig {
    std::string log_level;
    std::string log_file;
    bool performance_monitoring;
    size_t memory_pool_size;
    int thread_pool_size;
    std::map<std::string, ExchangeConfig> exchanges;
    std::vector<Instrument> instruments;
    ArbitrageConfig arbitrage;
};

// Utility functions
inline std::string exchangeToString(Exchange exchange) {
    switch (exchange) {
        case Exchange::OKX: return "OKX";
        case Exchange::BINANCE: return "BINANCE";
        case Exchange::BYBIT: return "BYBIT";
        default: return "UNKNOWN";
    }
}

inline Exchange stringToExchange(const std::string& str) {
    if (str == "OKX") return Exchange::OKX;
    if (str == "BINANCE") return Exchange::BINANCE;
    if (str == "BYBIT") return Exchange::BYBIT;
    return Exchange::UNKNOWN;
}

inline std::string instrumentTypeToString(InstrumentType type) {
    switch (type) {
        case InstrumentType::SPOT: return "SPOT";
        case InstrumentType::PERPETUAL_SWAP: return "PERPETUAL_SWAP";
        case InstrumentType::FUTURES: return "FUTURES";
        case InstrumentType::OPTION: return "OPTION";
        default: return "UNKNOWN";
    }
}

inline InstrumentType stringToInstrumentType(const std::string& str) {
    if (str == "SPOT") return InstrumentType::SPOT;
    if (str == "PERPETUAL_SWAP") return InstrumentType::PERPETUAL_SWAP;
    if (str == "FUTURES") return InstrumentType::FUTURES;
    if (str == "OPTION") return InstrumentType::OPTION;
    return InstrumentType::UNKNOWN;
}

inline Timestamp getCurrentTimestamp() {
    return std::chrono::high_resolution_clock::now();
}

inline double timestampToMs(const Timestamp& ts) {
    auto duration = ts.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / 1000.0;
}

} // namespace arbitrage
