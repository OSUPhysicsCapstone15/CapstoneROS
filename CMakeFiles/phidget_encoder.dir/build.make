# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/buckeye/catkin_ws/src/CapstoneROS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/buckeye/catkin_ws/src/CapstoneROS

# Include any dependencies generated for this target.
include CMakeFiles/phidget_encoder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/phidget_encoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/phidget_encoder.dir/flags.make

CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o: CMakeFiles/phidget_encoder.dir/flags.make
CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o: src/phidget_encoder.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/buckeye/catkin_ws/src/CapstoneROS/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o -c /home/buckeye/catkin_ws/src/CapstoneROS/src/phidget_encoder.cpp

CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/buckeye/catkin_ws/src/CapstoneROS/src/phidget_encoder.cpp > CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.i

CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/buckeye/catkin_ws/src/CapstoneROS/src/phidget_encoder.cpp -o CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.s

CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.requires:
.PHONY : CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.requires

CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.provides: CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.requires
	$(MAKE) -f CMakeFiles/phidget_encoder.dir/build.make CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.provides.build
.PHONY : CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.provides

CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.provides.build: CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o

# Object files for target phidget_encoder
phidget_encoder_OBJECTS = \
"CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o"

# External object files for target phidget_encoder
phidget_encoder_EXTERNAL_OBJECTS =

devel/lib/robot/phidget_encoder: CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o
devel/lib/robot/phidget_encoder: CMakeFiles/phidget_encoder.dir/build.make
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/libroscpp.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libboost_signals.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/librosconsole.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/librosconsole_log4cxx.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/librosconsole_backend_interface.so
devel/lib/robot/phidget_encoder: /usr/lib/liblog4cxx.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libboost_regex.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/libxmlrpcpp.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/libroscpp_serialization.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/librostime.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
devel/lib/robot/phidget_encoder: /opt/ros/indigo/lib/libcpp_common.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libboost_system.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libboost_thread.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libpthread.so
devel/lib/robot/phidget_encoder: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
devel/lib/robot/phidget_encoder: CMakeFiles/phidget_encoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable devel/lib/robot/phidget_encoder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/phidget_encoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/phidget_encoder.dir/build: devel/lib/robot/phidget_encoder
.PHONY : CMakeFiles/phidget_encoder.dir/build

CMakeFiles/phidget_encoder.dir/requires: CMakeFiles/phidget_encoder.dir/src/phidget_encoder.cpp.o.requires
.PHONY : CMakeFiles/phidget_encoder.dir/requires

CMakeFiles/phidget_encoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/phidget_encoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/phidget_encoder.dir/clean

CMakeFiles/phidget_encoder.dir/depend:
	cd /home/buckeye/catkin_ws/src/CapstoneROS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/buckeye/catkin_ws/src/CapstoneROS /home/buckeye/catkin_ws/src/CapstoneROS /home/buckeye/catkin_ws/src/CapstoneROS /home/buckeye/catkin_ws/src/CapstoneROS /home/buckeye/catkin_ws/src/CapstoneROS/CMakeFiles/phidget_encoder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/phidget_encoder.dir/depend

