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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/faradazerage/code/Dev/faradazerage/AndCam

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/faradazerage/code/Dev/faradazerage/AndCam/abuild

# Include any dependencies generated for this target.
include CMakeFiles/AndCam.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AndCam.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AndCam.dir/flags.make

CMakeFiles/AndCam.dir/AndCam.cpp.o: CMakeFiles/AndCam.dir/flags.make
CMakeFiles/AndCam.dir/AndCam.cpp.o: ../AndCam.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/faradazerage/code/Dev/faradazerage/AndCam/abuild/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/AndCam.dir/AndCam.cpp.o"
	/Users/faradazerage/code/android-dev/Toolchain/bin/arm-linux-androideabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/AndCam.dir/AndCam.cpp.o -c /Users/faradazerage/code/Dev/faradazerage/AndCam/AndCam.cpp

CMakeFiles/AndCam.dir/AndCam.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AndCam.dir/AndCam.cpp.i"
	/Users/faradazerage/code/android-dev/Toolchain/bin/arm-linux-androideabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/faradazerage/code/Dev/faradazerage/AndCam/AndCam.cpp > CMakeFiles/AndCam.dir/AndCam.cpp.i

CMakeFiles/AndCam.dir/AndCam.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AndCam.dir/AndCam.cpp.s"
	/Users/faradazerage/code/android-dev/Toolchain/bin/arm-linux-androideabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/faradazerage/code/Dev/faradazerage/AndCam/AndCam.cpp -o CMakeFiles/AndCam.dir/AndCam.cpp.s

CMakeFiles/AndCam.dir/AndCam.cpp.o.requires:
.PHONY : CMakeFiles/AndCam.dir/AndCam.cpp.o.requires

CMakeFiles/AndCam.dir/AndCam.cpp.o.provides: CMakeFiles/AndCam.dir/AndCam.cpp.o.requires
	$(MAKE) -f CMakeFiles/AndCam.dir/build.make CMakeFiles/AndCam.dir/AndCam.cpp.o.provides.build
.PHONY : CMakeFiles/AndCam.dir/AndCam.cpp.o.provides

CMakeFiles/AndCam.dir/AndCam.cpp.o.provides.build: CMakeFiles/AndCam.dir/AndCam.cpp.o

# Object files for target AndCam
AndCam_OBJECTS = \
"CMakeFiles/AndCam.dir/AndCam.cpp.o"

# External object files for target AndCam
AndCam_EXTERNAL_OBJECTS =

../libs/armeabi-v7a/libAndCam.so: CMakeFiles/AndCam.dir/AndCam.cpp.o
../libs/armeabi-v7a/libAndCam.so: CMakeFiles/AndCam.dir/build.make
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/rslam/rslam/CoreDev/libs/armeabi-v7a/libnode.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/rslam/rslam/CoreDev/libs/armeabi-v7a/libhal.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/rslam/rslam/CoreDev/libs/armeabi-v7a/libnode.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libboost_thread.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/rslam/rslam/CoreDev/libs/armeabi-v7a/libminiglog.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libzmq.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/rslam/rslam/CoreDev/libs/armeabi-v7a/libpbmsgs.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/arm-linux-androideabi/lib/libgnustl_shared.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libprotobuf.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_core.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_imgproc.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_features2d.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_flann.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_calib3d.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_objdetect.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_contrib.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_highgui.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_ml.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_video.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_legacy.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libopencv_java.so
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libboost_filesystem.a
../libs/armeabi-v7a/libAndCam.so: /Users/faradazerage/code/android-dev/Toolchain/user/lib/libboost_system.a
../libs/armeabi-v7a/libAndCam.so: CMakeFiles/AndCam.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../libs/armeabi-v7a/libAndCam.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AndCam.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AndCam.dir/build: ../libs/armeabi-v7a/libAndCam.so
.PHONY : CMakeFiles/AndCam.dir/build

CMakeFiles/AndCam.dir/requires: CMakeFiles/AndCam.dir/AndCam.cpp.o.requires
.PHONY : CMakeFiles/AndCam.dir/requires

CMakeFiles/AndCam.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AndCam.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AndCam.dir/clean

CMakeFiles/AndCam.dir/depend:
	cd /Users/faradazerage/code/Dev/faradazerage/AndCam/abuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/faradazerage/code/Dev/faradazerage/AndCam /Users/faradazerage/code/Dev/faradazerage/AndCam /Users/faradazerage/code/Dev/faradazerage/AndCam/abuild /Users/faradazerage/code/Dev/faradazerage/AndCam/abuild /Users/faradazerage/code/Dev/faradazerage/AndCam/abuild/CMakeFiles/AndCam.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AndCam.dir/depend

