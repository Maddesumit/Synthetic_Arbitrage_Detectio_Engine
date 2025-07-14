#pragma once

#include "types.hpp"
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <mutex>

namespace arbitrage {

class PerformanceMonitor {
public:
    static PerformanceMonitor& getInstance();
    
    // Initialize performance monitoring
    bool initialize(int monitoring_interval_ms = 1000);
    
    // Start/stop monitoring
    void start();
    void stop();
    
    // Metrics recording
    void recordMessageProcessed();
    void recordOpportunityDetected();
    void recordTradeExecuted();
    void recordLatency(double latency_ms);
    void recordMemoryUsage(double memory_mb);
    void recordCpuUsage(double cpu_percentage);
    
    // Get current metrics
    PerformanceMetrics getMetrics() const;
    
    // Reset metrics
    void resetMetrics();
    
    // Get specific metrics
    uint64_t getMessagesProcessed() const;
    uint64_t getOpportunitiesDetected() const;
    uint64_t getTradesExecuted() const;
    double getAverageLatency() const;
    double getMaxLatency() const;
    double getMemoryUsage() const;
    double getCpuUsage() const;
    
    // Performance thresholds
    bool isLatencyWithinThreshold(double threshold_ms) const;
    bool isMemoryWithinThreshold(double threshold_mb) const;
    bool isCpuWithinThreshold(double threshold_percentage) const;
    
    // Callbacks for alerts
    using AlertCallback = std::function<void(const std::string&, const std::string&)>;
    void setLatencyAlertCallback(AlertCallback callback, double threshold_ms);
    void setMemoryAlertCallback(AlertCallback callback, double threshold_mb);
    void setCpuAlertCallback(AlertCallback callback, double threshold_percentage);
    
private:
    PerformanceMonitor() = default;
    ~PerformanceMonitor();
    PerformanceMonitor(const PerformanceMonitor&) = delete;
    PerformanceMonitor& operator=(const PerformanceMonitor&) = delete;
    
    // Background monitoring thread
    void monitoringLoop();
    
    // System metrics collection
    double getCurrentMemoryUsage();
    double getCurrentCpuUsage();
    
    // Alert checking
    void checkAlerts();
    
    AtomicPerformanceMetrics metrics_;
    std::atomic<bool> running_{false};
    std::atomic<bool> monitoring_enabled_{false};
    std::unique_ptr<std::thread> monitoring_thread_;
    int monitoring_interval_ms_{1000};
    
    // Latency tracking
    std::atomic<uint64_t> latency_count_{0};
    std::atomic<double> latency_sum_{0.0};
    
    // Alert thresholds and callbacks
    AlertCallback latency_alert_callback_;
    AlertCallback memory_alert_callback_;
    AlertCallback cpu_alert_callback_;
    double latency_threshold_ms_{0.0};
    double memory_threshold_mb_{0.0};
    double cpu_threshold_percentage_{0.0};
    
    // System monitoring
    std::chrono::steady_clock::time_point last_cpu_check_;
    uint64_t last_cpu_time_{0};
    
    mutable std::mutex metrics_mutex_;
};

} // namespace arbitrage
