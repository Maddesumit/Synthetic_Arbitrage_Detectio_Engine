# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /workspaces/Synthetic_Arbitrage_Detectio_Engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /workspaces/Synthetic_Arbitrage_Detectio_Engine/build

# Include any dependencies generated for this target.
include CMakeFiles/SyntheticArbitrageEngine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SyntheticArbitrageEngine.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SyntheticArbitrageEngine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SyntheticArbitrageEngine.dir/flags.make

CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/flags.make
CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o: /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/main.cpp
CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/workspaces/Synthetic_Arbitrage_Detectio_Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o -MF CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o.d -o CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o -c /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/main.cpp

CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/main.cpp > CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.i

CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/main.cpp -o CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.s

CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/flags.make
CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o: /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/core/config_manager.cpp
CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/workspaces/Synthetic_Arbitrage_Detectio_Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o -MF CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o.d -o CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o -c /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/core/config_manager.cpp

CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/core/config_manager.cpp > CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.i

CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/core/config_manager.cpp -o CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.s

CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/flags.make
CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o: /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/logger.cpp
CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/workspaces/Synthetic_Arbitrage_Detectio_Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o -MF CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o.d -o CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o -c /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/logger.cpp

CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/logger.cpp > CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.i

CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/logger.cpp -o CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.s

CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/flags.make
CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o: /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/performance_monitor.cpp
CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o: CMakeFiles/SyntheticArbitrageEngine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/workspaces/Synthetic_Arbitrage_Detectio_Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o -MF CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o.d -o CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o -c /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/performance_monitor.cpp

CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/performance_monitor.cpp > CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.i

CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /workspaces/Synthetic_Arbitrage_Detectio_Engine/src/utils/performance_monitor.cpp -o CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.s

# Object files for target SyntheticArbitrageEngine
SyntheticArbitrageEngine_OBJECTS = \
"CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o" \
"CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o" \
"CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o" \
"CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o"

# External object files for target SyntheticArbitrageEngine
SyntheticArbitrageEngine_EXTERNAL_OBJECTS =

SyntheticArbitrageEngine: CMakeFiles/SyntheticArbitrageEngine.dir/src/main.cpp.o
SyntheticArbitrageEngine: CMakeFiles/SyntheticArbitrageEngine.dir/src/core/config_manager.cpp.o
SyntheticArbitrageEngine: CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/logger.cpp.o
SyntheticArbitrageEngine: CMakeFiles/SyntheticArbitrageEngine.dir/src/utils/performance_monitor.cpp.o
SyntheticArbitrageEngine: CMakeFiles/SyntheticArbitrageEngine.dir/build.make
SyntheticArbitrageEngine: /opt/conda/lib/libspdlog.so.1.11.0
SyntheticArbitrageEngine: /opt/conda/lib/libfmt.so.9.1.0
SyntheticArbitrageEngine: CMakeFiles/SyntheticArbitrageEngine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/workspaces/Synthetic_Arbitrage_Detectio_Engine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable SyntheticArbitrageEngine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SyntheticArbitrageEngine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SyntheticArbitrageEngine.dir/build: SyntheticArbitrageEngine
.PHONY : CMakeFiles/SyntheticArbitrageEngine.dir/build

CMakeFiles/SyntheticArbitrageEngine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SyntheticArbitrageEngine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SyntheticArbitrageEngine.dir/clean

CMakeFiles/SyntheticArbitrageEngine.dir/depend:
	cd /workspaces/Synthetic_Arbitrage_Detectio_Engine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /workspaces/Synthetic_Arbitrage_Detectio_Engine /workspaces/Synthetic_Arbitrage_Detectio_Engine /workspaces/Synthetic_Arbitrage_Detectio_Engine/build /workspaces/Synthetic_Arbitrage_Detectio_Engine/build /workspaces/Synthetic_Arbitrage_Detectio_Engine/build/CMakeFiles/SyntheticArbitrageEngine.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/SyntheticArbitrageEngine.dir/depend

