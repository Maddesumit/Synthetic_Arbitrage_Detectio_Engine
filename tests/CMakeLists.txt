cmake_minimum_required(VERSION 3.16)

# Find GTest
find_package(GTest REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Test source files
file(GLOB_RECURSE TEST_SOURCES 
    "test_*.cpp"
    "*_test.cpp"
)

# Create test executable
add_executable(unit_tests ${TEST_SOURCES})

# Add test sources from main project (excluding main.cpp)
file(GLOB_RECURSE MAIN_SOURCES 
    "../src/core/*.cpp"
    "../src/exchanges/*.cpp"
    "../src/pricing/*.cpp"
    "../src/detection/*.cpp"
    "../src/risk/*.cpp"
    "../src/utils/*.cpp"
)

target_sources(unit_tests PRIVATE ${MAIN_SOURCES})

# Link libraries
target_link_libraries(unit_tests 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

# Link nlohmann_json if found as package
if(nlohmann_json_FOUND)
    target_link_libraries(unit_tests nlohmann_json::nlohmann_json)
endif()

# Link spdlog
if(spdlog_FOUND)
    target_link_libraries(unit_tests spdlog::spdlog)
elseif(SPDLOG_LIBRARY)
    target_link_libraries(unit_tests ${SPDLOG_LIBRARY})
endif()

# Link fmt
if(fmt_FOUND)
    target_link_libraries(unit_tests fmt::fmt)
elseif(FMT_LIBRARY)
    target_link_libraries(unit_tests ${FMT_LIBRARY})
endif()

# Register tests with CTest
add_test(NAME unit_tests COMMAND unit_tests)

# Enable testing
enable_testing()
