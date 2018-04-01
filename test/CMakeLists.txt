# Copyright 2017 Jarrod N. Bakker
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# CMakeLists.txt file for the test for the C++ implementation of nmeta2 DPAE.
#
# Author: Jarrod N. Bakker
# Date: 27/12/2017
#

cmake_minimum_required(VERSION 3.5.1)
project(nmeta2dpae)

## Specify where the binary/executable will be outputted to
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

## Add the header files and include binary tree to the search path for the
# nm2dpaeBuildSettings.h file.
include_directories(src/Config src/ext/rapid_json src/ext/spdlog "${PROJECT_BINARY_DIR}")

## Add the source code files
#set(SOURCES src/nmeta2dpae.cpp src/config/config.cpp src/config/tc_policy.cpp
#    src/data_plane/data_plane_services.cpp src/data_plane/traffic_classification.cpp
#    src/data_plane/sniff.cpp src/control_plane/control_plane_services.cpp
#    src/util/json_body.cpp src/util/logging_util.cpp)

## configure the compiler
set (CMAKE_CXX_STANDARD 11)

## Include libraries
FIND_PACKAGE(yaml-cpp REQUIRED)

## Configure CMake for including Google Test (Google C++ Testing Framework).
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

## Add the test code files
set(TESTS t_src/test_json_body.cpp)

## Add the executable
add_executable(nmeta2dpae_test ${SOURCES} ${TESTS})

## Link libraries to the executable
# pthread is included so that spdlog can run.
target_link_libraries(nmeta2dpae_test pthread yaml-cpp gtest_main)