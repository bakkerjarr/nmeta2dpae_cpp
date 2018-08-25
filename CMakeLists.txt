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
# CMakeLists.txt file for the C++ implementation of nmeta2 DPAE.
#
# Author: Jarrod N. Bakker
# Date: 27/12/2017
#

cmake_minimum_required(VERSION 3.5.1)
project(nmeta2dpae)

## Specify where the binary/executable will be outputted to
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

## Version number for nmeta2dpae
# Declare the version number
set (nmeta2dpae_VERSION_MAJOR 0)
set (nmeta2dpae_VERSION_MINOR 0)
set (nmeta2dpae_VERSION_PATCH 1)
set (nmeta2dpae_VERSION "${nmeta2dpae_VERSION_MAJOR}.${nmeta2dpae_VERSION_MINOR}.${nmeta2dpae_VERSION_PATCH}")
# Configure a header file for passing the version numbers to the source code
configure_file (
    "src/nmeta2dpaeBuildSettings.hpp.in"
    "${PROJECT_BINARY_DIR}/nmeta2dpaeBuildSettings.hpp"
)

## Add the header files and include binary tree to the search path for the
# nm2dpaeBuildSettings.h file.
include_directories(src/Config src/ext/rapid_json src/ext/spdlog "${PROJECT_BINARY_DIR}")

## Add the source code files
set(SOURCES src/nmeta2dpae.cpp src/config/config.cpp src/config/tc_policy.cpp
    src/data_plane/data_plane_services.cpp src/data_plane/traffic_classification.cpp
    src/data_plane/sniff.cpp
    src/control_plane/control_plane_services.cpp
    src/control_plane/control_plane_api_mgmt.cpp
    src/util/json_body.cpp src/util/logging_util.cpp)

## configure the compiler
set (CMAKE_CXX_STANDARD 11)

## Include libraries
FIND_PACKAGE(yaml-cpp REQUIRED)

## Add the executable
add_executable(nmeta2dpae ${SOURCES})

## Link libraries to the executable
# pthread is included so that spdlog can run.
target_link_libraries(nmeta2dpae curl uuid pthread yaml-cpp)