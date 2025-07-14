cmake_minimum_required(VERSION 3.16)
project(SyntheticArbitrageEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3 -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

# Find nlohmann_json
find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
    find_path(NLOHMANN_JSON_INCLUDE_DIR nlohmann/json.hpp)
    if(NLOHMANN_JSON_INCLUDE_DIR)
        message(STATUS "Found nlohmann/json header at: ${NLOHMANN_JSON_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "nlohmann/json not found. Please install nlohmann-json3-dev")
    endif()
endif()

# Find spdlog
find_package(spdlog QUIET)
if(NOT spdlog_FOUND)
    find_path(SPDLOG_INCLUDE_DIR spdlog/spdlog.h)
    find_library(SPDLOG_LIBRARY spdlog)
    if(SPDLOG_INCLUDE_DIR AND SPDLOG_LIBRARY)
        message(STATUS "Found spdlog header at: ${SPDLOG_INCLUDE_DIR}")
        message(STATUS "Found spdlog library at: ${SPDLOG_LIBRARY}")
    else()
        message(FATAL_ERROR "spdlog not found. Please install libspdlog-dev")
    endif()
endif()

# Find fmt
find_package(fmt QUIET)
if(NOT fmt_FOUND)
    find_path(FMT_INCLUDE_DIR fmt/format.h)
    find_library(FMT_LIBRARY fmt)
    if(FMT_INCLUDE_DIR AND FMT_LIBRARY)
        message(STATUS "Found fmt header at: ${FMT_INCLUDE_DIR}")
        message(STATUS "Found fmt library at: ${FMT_LIBRARY}")
    else()
        message(FATAL_ERROR "fmt not found. Please install libfmt-dev")
    endif()
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add system include directories if found manually
if(NLOHMANN_JSON_INCLUDE_DIR)
    include_directories(${NLOHMANN_JSON_INCLUDE_DIR})
endif()
if(SPDLOG_INCLUDE_DIR)
    include_directories(${SPDLOG_INCLUDE_DIR})
endif()
if(FMT_INCLUDE_DIR)
    include_directories(${FMT_INCLUDE_DIR})
endif()

# Source files
file(GLOB_RECURSE SOURCES 
    "src/core/*.cpp"
    "src/exchanges/*.cpp"
    "src/pricing/*.cpp"
    "src/detection/*.cpp"
    "src/risk/*.cpp"
    "src/utils/*.cpp"
)

# Header files
file(GLOB_RECURSE HEADERS 
    "include/*.h"
    "include/*.hpp"
    "src/*.h"
    "src/*.hpp"
)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

# Link nlohmann_json if found as package
if(nlohmann_json_FOUND)
    target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
endif()

# Link spdlog
if(spdlog_FOUND)
    target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
elseif(SPDLOG_LIBRARY)
    target_link_libraries(${PROJECT_NAME} ${SPDLOG_LIBRARY})
endif()

# Link fmt
if(fmt_FOUND)
    target_link_libraries(${PROJECT_NAME} fmt::fmt)
elseif(FMT_LIBRARY)
    target_link_libraries(${PROJECT_NAME} ${FMT_LIBRARY})
endif()

# Enable testing
enable_testing()
add_subdirectory(tests)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Package configuration
set(CPACK_PACKAGE_NAME "SyntheticArbitrageEngine")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "High-performance synthetic arbitrage detection engine")
set(CPACK_PACKAGE_CONTACT "dev@arbitrage-engine.com")
include(CPack)
