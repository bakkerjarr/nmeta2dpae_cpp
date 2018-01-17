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
project(nm2dpae)

### Version number for nmeta2dpae
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

# Add the header files and include binary tree to the search path for the
# nm2dpaeBuildSettings.h file.
include_directories(src/Config "${PROJECT_BINARY_DIR}")

# Add the source code files
set(SOURCES src/nmeta2dpae.cpp src/Config/config.cpp)

# configure the compiler
set (CMAKE_CXX_STANDARD 11)

# Include libraries
FIND_PACKAGE(yaml-cpp REQUIRED)

# Add the executable
add_executable(nm2dpae ${SOURCES})

# Link libraries to the executable
TARGET_LINK_LIBRARIES(nm2dpae yaml-cpp)