# Synthetic Arbitrage Detection Engine

A high-performance C++ arbitrage detection engine that identifies mispricings across synthetic and real instrument pairs using live market data from multiple exchanges (OKX, Binance, Bybit).

## 🚀 Project Status

**Current Phase**: Phase 1 - Foundation and Environment Setup ✅ **COMPLETED**

**Next Phase**: Phase 2 - WebSocket Infrastructure and Market Data Pipeline

## 🏗️ Architecture Overview

The engine is designed to detect arbitrage opportunities across:
- **Real vs Synthetic Spots**: Spot price vs synthetic derivative construction
- **Real vs Synthetic Derivatives**: Actual derivative price vs synthetic construction  
- **Cross-Synthetic Opportunities**: Multi-leg synthetic constructions

## 📊 Key Features

### ✅ Phase 1 Completed
- **High-Performance Foundation**: C++20 with atomic operations and lock-free data structures
- **Configuration Management**: JSON-based configuration with runtime validation
- **Advanced Logging**: Multi-level logging with specialized loggers for different components
- **Performance Monitoring**: Real-time metrics collection with configurable alerts
- **Type System**: Comprehensive data structures for market data and arbitrage opportunities
- **Unit Testing**: Google Test framework with comprehensive test coverage
- **Build System**: CMake with vcpkg dependency management

### 🔄 Upcoming Phases
- **Phase 2**: WebSocket connections to OKX, Binance, Bybit exchanges
- **Phase 3**: Synthetic pricing models (perpetual swaps, futures, options)
- **Phase 4**: Arbitrage detection algorithms and signal generation
- **Phase 5**: Risk management and position sizing
- **Phase 6**: Performance optimization and advanced features
- **Phase 7**: Testing, validation, and production deployment

## 🛠️ Technical Specifications

### Performance Targets
- **Latency**: <10ms arbitrage detection
- **Throughput**: 2000+ market updates per second
- **Memory**: <2GB total usage
- **Accuracy**: >99.5% pricing model accuracy

### Technology Stack
- **Language**: C++20
- **Build System**: CMake 3.16+
- **Dependencies**: nlohmann/json, spdlog, Boost.Beast (planned)
- **Testing**: Google Test
- **Package Management**: vcpkg

### Supported Exchanges
- **OKX**: Public WebSocket API for spot and derivatives
- **Binance**: Spot and futures WebSocket APIs
- **Bybit**: Spot and derivatives WebSocket APIs

## 🚀 Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config

# Install vcpkg (optional but recommended)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

### Build Instructions
```bash
# Clone the repository
git clone <repository-url>
cd Synthetic_Arbitrage_Detectio_Engine

# Build the project
chmod +x build.sh
./build.sh
```

### Run the Application
```bash
cd build
./SyntheticArbitrageEngine config/engine_config.json
```

### Run Tests
```bash
cd build
make test
# or directly
./tests/unit_tests
```

## 📋 Configuration

The engine uses JSON configuration files. Example configuration:

```json
{
  "system": {
    "log_level": "info",
    "thread_pool_size": 8,
    "memory_pool_size": 1073741824
  },
  "exchanges": {
    "okx": {
      "enabled": true,
      "websocket_url": "wss://ws.okx.com:8443/ws/v5/public",
      "rate_limit": {
        "requests_per_second": 20,
        "burst_size": 100
      }
    }
  },
  "arbitrage": {
    "detection": {
      "min_profit_threshold": 0.001,
      "max_latency_ms": 10
    },
    "risk_management": {
      "max_position_size": 10000.0,
      "max_leverage": 10.0
    }
  }
}
```

## 📈 Performance Monitoring

The engine includes comprehensive performance monitoring:

- **Real-time Metrics**: Latency, throughput, memory usage, CPU usage
- **Configurable Alerts**: Threshold-based alerts with callbacks
- **Specialized Logging**: Separate logs for performance, market data, trades, and risk
- **System Metrics**: Memory and CPU usage tracking

## 🧪 Testing

The project includes comprehensive unit tests covering:
- Configuration loading and validation
- Performance monitoring functionality
- Type system and utilities
- Logging system integration
- Memory and CPU usage tracking

## 📖 Documentation

- **[Phase 1 Documentation](docs/phase1_documentation.md)**: Complete documentation of implemented features
- **[Project Plan](PLAN.md)**: Detailed 7-phase development plan
- **[API Documentation]**: Coming in Phase 2

## 🔧 Development

### Project Structure
```
src/
├── core/           # Core engine components
├── exchanges/      # Exchange-specific implementations
├── pricing/        # Synthetic pricing models
├── detection/      # Arbitrage detection algorithms
├── risk/          # Risk management components
├── utils/         # Utility functions
└── main.cpp       # Entry point

include/           # Header files
tests/            # Unit tests
config/           # Configuration files
docs/             # Documentation
```

### Key Components
- **ConfigManager**: Thread-safe configuration management
- **Logger**: Multi-level logging with file rotation
- **PerformanceMonitor**: Real-time metrics collection
- **Type System**: Market data and arbitrage data structures

## 🎯 Roadmap

### Phase 2 (Weeks 3-4): WebSocket Infrastructure
- [ ] WebSocket connection manager
- [ ] Exchange-specific WebSocket clients
- [ ] Market data processing pipeline
- [ ] Data synchronization mechanisms

### Phase 3 (Weeks 5-6): Synthetic Pricing Models
- [ ] Perpetual swap synthetic pricing
- [ ] Futures synthetic pricing
- [ ] Options synthetic pricing
- [ ] Cross-exchange price normalization

### Phase 4 (Weeks 7-8): Arbitrage Detection
- [ ] Real vs synthetic spot detection
- [ ] Real vs synthetic derivatives detection
- [ ] Cross-synthetic opportunities
- [ ] Signal generation and filtering

### Phase 5 (Weeks 9-10): Risk Management
- [ ] Position sizing algorithms
- [ ] Real-time risk monitoring
- [ ] VaR calculations
- [ ] Correlation analysis

### Phase 6 (Weeks 11-12): Performance Optimization
- [ ] SIMD optimizations
- [ ] Memory pool implementation
- [ ] Lock-free data structures
- [ ] Network optimization

### Phase 7 (Weeks 13-14): Production Deployment
- [ ] Comprehensive testing
- [ ] Performance benchmarking
- [ ] Production deployment
- [ ] Monitoring and alerting

## 🚨 Important Notes

- **Live Market Data Only**: The system is designed to work exclusively with live market data from exchanges. No demo or dummy data is used.
- **High-Performance Requirements**: The system must process 2000+ market updates per second with <10ms detection latency.
- **Production-Ready**: The architecture is designed for production deployment with comprehensive monitoring and error handling.

## 🤝 Contributing

This is a structured development project following the 7-phase plan. Each phase builds upon the previous one, ensuring systematic development and testing.

## 📄 License

[Add your license information here]

## 📞 Contact

[Add your contact information here]

---

**Current Status**: Phase 1 Complete ✅ | **Next**: Phase 2 WebSocket Infrastructure 🔄