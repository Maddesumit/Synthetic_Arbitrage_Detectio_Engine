#include "performance_monitor.hpp"
#include "logger.hpp"
#include <fstream>
#include <sstream>
#include <sys/resource.h>
#include <unistd.h>
#include <ios>
#include <mutex>

namespace arbitrage {

PerformanceMonitor& PerformanceMonitor::getInstance() {
    static PerformanceMonitor instance;
    return instance;
}

PerformanceMonitor::~PerformanceMonitor() {
    stop();
}

bool PerformanceMonitor::initialize(int monitoring_interval_ms) {
    monitoring_interval_ms_ = monitoring_interval_ms;
    
    // Initialize system monitoring
    last_cpu_check_ = std::chrono::steady_clock::now();
    last_cpu_time_ = 0;
    
    LOG_INFO("Performance monitor initialized with {}ms interval", monitoring_interval_ms);
    return true;
}

void PerformanceMonitor::start() {
    if (running_.exchange(true)) {
        LOG_WARN("Performance monitor already running");
        return;
    }
    
    monitoring_enabled_ = true;
    monitoring_thread_ = std::make_unique<std::thread>(&PerformanceMonitor::monitoringLoop, this);
    
    LOG_INFO("Performance monitor started");
}

void PerformanceMonitor::stop() {
    if (!running_.exchange(false)) {
        return;
    }
    
    monitoring_enabled_ = false;
    
    if (monitoring_thread_ && monitoring_thread_->joinable()) {
        monitoring_thread_->join();
    }
    
    LOG_INFO("Performance monitor stopped");
}

void PerformanceMonitor::recordMessageProcessed() {
    metrics_.messages_processed.fetch_add(1, std::memory_order_relaxed);
}

void PerformanceMonitor::recordOpportunityDetected() {
    metrics_.opportunities_detected.fetch_add(1, std::memory_order_relaxed);
}

void PerformanceMonitor::recordTradeExecuted() {
    metrics_.trades_executed.fetch_add(1, std::memory_order_relaxed);
}

void PerformanceMonitor::recordLatency(double latency_ms) {
    // Update max latency
    double current_max = metrics_.max_latency_ms.load(std::memory_order_relaxed);
    while (latency_ms > current_max && 
           !metrics_.max_latency_ms.compare_exchange_weak(current_max, latency_ms, std::memory_order_relaxed)) {
        // Keep trying until we successfully update or find a larger value
    }
    
    // Update running average with mutex protection
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    uint64_t count = latency_count_.fetch_add(1, std::memory_order_relaxed) + 1;
    double current_sum = latency_sum_.load(std::memory_order_relaxed);
    double new_sum = current_sum + latency_ms;
    latency_sum_.store(new_sum, std::memory_order_relaxed);
    metrics_.average_latency_ms.store(new_sum / count, std::memory_order_relaxed);
}

void PerformanceMonitor::recordMemoryUsage(double memory_mb) {
    metrics_.memory_usage_mb.store(memory_mb, std::memory_order_relaxed);
}

void PerformanceMonitor::recordCpuUsage(double cpu_percentage) {
    metrics_.cpu_usage_percentage.store(cpu_percentage, std::memory_order_relaxed);
}

PerformanceMetrics PerformanceMonitor::getMetrics() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    PerformanceMetrics current_metrics;
    
    // Copy atomic values to non-atomic fields
    current_metrics.messages_processed = metrics_.messages_processed.load();
    current_metrics.opportunities_detected = metrics_.opportunities_detected.load();
    current_metrics.trades_executed = metrics_.trades_executed.load();
    current_metrics.average_latency_ms = metrics_.average_latency_ms.load();
    current_metrics.max_latency_ms = metrics_.max_latency_ms.load();
    current_metrics.memory_usage_mb = metrics_.memory_usage_mb.load();
    current_metrics.cpu_usage_percentage = metrics_.cpu_usage_percentage.load();
    current_metrics.last_update = getCurrentTimestamp();
    
    return current_metrics;
}

void PerformanceMonitor::resetMetrics() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    metrics_.reset();
    latency_count_ = 0;
    latency_sum_ = 0.0;
    LOG_INFO("Performance metrics reset");
}

uint64_t PerformanceMonitor::getMessagesProcessed() const {
    return metrics_.messages_processed.load(std::memory_order_relaxed);
}

uint64_t PerformanceMonitor::getOpportunitiesDetected() const {
    return metrics_.opportunities_detected.load(std::memory_order_relaxed);
}

uint64_t PerformanceMonitor::getTradesExecuted() const {
    return metrics_.trades_executed.load(std::memory_order_relaxed);
}

double PerformanceMonitor::getAverageLatency() const {
    return metrics_.average_latency_ms.load(std::memory_order_relaxed);
}

double PerformanceMonitor::getMaxLatency() const {
    return metrics_.max_latency_ms.load(std::memory_order_relaxed);
}

double PerformanceMonitor::getMemoryUsage() const {
    return metrics_.memory_usage_mb.load(std::memory_order_relaxed);
}

double PerformanceMonitor::getCpuUsage() const {
    return metrics_.cpu_usage_percentage.load(std::memory_order_relaxed);
}

bool PerformanceMonitor::isLatencyWithinThreshold(double threshold_ms) const {
    return getAverageLatency() <= threshold_ms;
}

bool PerformanceMonitor::isMemoryWithinThreshold(double threshold_mb) const {
    return getMemoryUsage() <= threshold_mb;
}

bool PerformanceMonitor::isCpuWithinThreshold(double threshold_percentage) const {
    return getCpuUsage() <= threshold_percentage;
}

void PerformanceMonitor::setLatencyAlertCallback(AlertCallback callback, double threshold_ms) {
    latency_alert_callback_ = callback;
    latency_threshold_ms_ = threshold_ms;
}

void PerformanceMonitor::setMemoryAlertCallback(AlertCallback callback, double threshold_mb) {
    memory_alert_callback_ = callback;
    memory_threshold_mb_ = threshold_mb;
}

void PerformanceMonitor::setCpuAlertCallback(AlertCallback callback, double threshold_percentage) {
    cpu_alert_callback_ = callback;
    cpu_threshold_percentage_ = threshold_percentage;
}

void PerformanceMonitor::monitoringLoop() {
    while (monitoring_enabled_) {
        try {
            // Update system metrics
            recordMemoryUsage(getCurrentMemoryUsage());
            recordCpuUsage(getCurrentCpuUsage());
            
            // Check alerts
            checkAlerts();
            
            // Log performance metrics
            auto metrics = getMetrics();
            LOG_PERFORMANCE("Messages: {}, Opportunities: {}, Trades: {}, "
                          "Avg Latency: {:.2f}ms, Max Latency: {:.2f}ms, "
                          "Memory: {:.2f}MB, CPU: {:.2f}%",
                          metrics.messages_processed,
                          metrics.opportunities_detected,
                          metrics.trades_executed,
                          metrics.average_latency_ms,
                          metrics.max_latency_ms,
                          metrics.memory_usage_mb,
                          metrics.cpu_usage_percentage);
            
            // Sleep for the monitoring interval
            std::this_thread::sleep_for(std::chrono::milliseconds(monitoring_interval_ms_));
            
        } catch (const std::exception& e) {
            LOG_ERROR("Error in performance monitoring loop: {}", e.what());
        }
    }
}

double PerformanceMonitor::getCurrentMemoryUsage() {
    try {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        
        // Convert from KB to MB
        return usage.ru_maxrss / 1024.0;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to get memory usage: {}", e.what());
        return 0.0;
    }
}

double PerformanceMonitor::getCurrentCpuUsage() {
    try {
        std::ifstream stat_file("/proc/stat");
        if (!stat_file.is_open()) {
            return 0.0;
        }
        
        std::string line;
        std::getline(stat_file, line);
        
        // Parse CPU time from /proc/stat
        std::istringstream iss(line);
        std::string cpu_label;
        uint64_t user, nice, system, idle, iowait, irq, softirq, steal;
        
        iss >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        
        uint64_t total_time = user + nice + system + idle + iowait + irq + softirq + steal;
        uint64_t idle_time = idle + iowait;
        
        auto current_time = std::chrono::steady_clock::now();
        auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(
            current_time - last_cpu_check_).count();
        
        if (time_diff > 0 && last_cpu_time_ > 0) {
            uint64_t total_diff = total_time - last_cpu_time_;
            uint64_t idle_diff = idle_time - (last_cpu_time_ - (total_time - idle_time));
            
            if (total_diff > 0) {
                double cpu_usage = 100.0 * (1.0 - static_cast<double>(idle_diff) / total_diff);
                last_cpu_time_ = total_time;
                last_cpu_check_ = current_time;
                return cpu_usage;
            }
        }
        
        last_cpu_time_ = total_time;
        last_cpu_check_ = current_time;
        return 0.0;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to get CPU usage: {}", e.what());
        return 0.0;
    }
}

void PerformanceMonitor::checkAlerts() {
    // Check latency alert
    if (latency_alert_callback_ && latency_threshold_ms_ > 0) {
        double current_latency = getAverageLatency();
        if (current_latency > latency_threshold_ms_) {
            latency_alert_callback_("LATENCY_ALERT", 
                std::string("Average latency ") + std::to_string(current_latency) + 
                "ms exceeds threshold " + std::to_string(latency_threshold_ms_) + "ms");
        }
    }
    
    // Check memory alert
    if (memory_alert_callback_ && memory_threshold_mb_ > 0) {
        double current_memory = getMemoryUsage();
        if (current_memory > memory_threshold_mb_) {
            memory_alert_callback_("MEMORY_ALERT", 
                std::string("Memory usage ") + std::to_string(current_memory) + 
                "MB exceeds threshold " + std::to_string(memory_threshold_mb_) + "MB");
        }
    }
    
    // Check CPU alert
    if (cpu_alert_callback_ && cpu_threshold_percentage_ > 0) {
        double current_cpu = getCpuUsage();
        if (current_cpu > cpu_threshold_percentage_) {
            cpu_alert_callback_("CPU_ALERT", 
                std::string("CPU usage ") + std::to_string(current_cpu) + 
                "% exceeds threshold " + std::to_string(cpu_threshold_percentage_) + "%");
        }
    }
}

} // namespace arbitrage
