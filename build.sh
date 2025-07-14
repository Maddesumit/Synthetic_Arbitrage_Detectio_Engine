#!/bin/bash

# Build script for Synthetic Arbitrage Detection Engine
# Phase 1: Foundation and Environment Setup

set -e  # Exit on any error

echo "=== Synthetic Arbitrage Detection Engine Build Script ==="
echo "Phase 1: Foundation and Environment Setup"
echo ""

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the project root."
    exit 1
fi

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo "Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

echo "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Check for required tools
echo "Checking for required tools..."

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake not found. Please install CMake 3.16 or later."
    exit 1
fi

# Check for C++ compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "Error: C++ compiler not found. Please install GCC or Clang."
    exit 1
fi

# Check compiler version
if command -v g++ &> /dev/null; then
    GCC_VERSION=$(g++ --version | head -n1 | grep -oE '[0-9]+\.[0-9]+' | head -n1)
    echo "Found GCC version: $GCC_VERSION"
    
    # Check if version is >= 9.0
    if [ "$(printf '%s\n' "9.0" "$GCC_VERSION" | sort -V | head -n1)" != "9.0" ]; then
        echo "Warning: GCC version 9.0 or later is recommended for C++20 support."
    fi
fi

# Check for vcpkg (optional)
if command -v vcpkg &> /dev/null; then
    echo "Found vcpkg - using for dependency management"
    CMAKE_ARGS="-DCMAKE_TOOLCHAIN_FILE=$(vcpkg --vcpkg-root)/scripts/buildsystems/vcpkg.cmake"
else
    echo "vcpkg not found - will use system packages"
    CMAKE_ARGS=""
fi

# Configure build
echo ""
echo "Configuring build..."
cmake .. $CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_STANDARD=17 \
    -DCMAKE_CXX_STANDARD_REQUIRED=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build project
echo ""
echo "Building project..."
make -j$(nproc) || make -j4  # Use all cores, fallback to 4 if nproc fails

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo ""
    echo "=== Build completed successfully! ==="
    echo ""
    echo "Executables created:"
    ls -la SyntheticArbitrageEngine 2>/dev/null || echo "  Main executable: build/SyntheticArbitrageEngine"
    ls -la tests/unit_tests 2>/dev/null || echo "  Unit tests: build/tests/unit_tests"
    
    echo ""
    echo "To run the application:"
    echo "  cd /workspaces/Synthetic_Arbitrage_Detectio_Engine"
    echo "  LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:\$LD_LIBRARY_PATH ./build/SyntheticArbitrageEngine [config_file]"
    echo ""
    echo "To run unit tests:"
    echo "  cd /workspaces/Synthetic_Arbitrage_Detectio_Engine"
    echo "  LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:\$LD_LIBRARY_PATH ./build/tests/unit_tests"
    echo ""
    echo "Phase 1 setup completed successfully!"
else
    echo ""
    echo "=== Build failed! ==="
    echo "Please check the error messages above and fix any issues."
    exit 1
fi
