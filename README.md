# nmeta2dpae_cpp
This repository contains a C++ implementation of the Data Plane Auxiliary Engine (DPAE) for use with nmeta2.

## Build Instructions

Run the following command from within the home directory for the project i.e. *nmeta2dpae_cpp*

```bash
$ cmake -H. -Bbuild
```

## Links
The repository containing the source and license for the original nmeta2 DPAE implementation can be found at https://github.com/mattjhayes/nmeta2dpae.

The repository containing the source and license for the nmeta2 project can be found at https://github.com/mattjhayes/nmeta2.

## Testing
This repository utilises the Google C++ Testing Framework: *Google Test*. The GitHub repository for Google Test can be found at https://github.com/google/googletest.

The *test* directory contains a separate CMakeLists.txt file for building the tests.

## Required Libraries
### libcurl4-openssl-dev
```bash
$ sudo apt-get install libcurl4-openssl-dev
```

### libtbb-dev (Intel Threading Building Blocks)
```bash
$ sudo apt-get install libtbb-dev
```

### RapidJSON version 1.1.0
RapidJSON is a header-only library (https://github.com/Tencent/rapidjson/tree/version1.1.0). It was installed by copy the source folder (https://github.com/Tencent/rapidjson/tree/version1.1.0/include/rapidjson) into the build tree for this implementation of nmeta2 DPAE. The CMakeList.txt includes the library through the ```include_directories``` directive.

### spdlog version 0.16.3
spdlog is a header-only library (https://github.com/gabime/spdlog). It was installed by copying the source folder (https://github.com/gabime/spdlog/tree/master/include/spdlog) into the build tree for this implementation of nmeta2 DPAE. After making the necessary change to the CMakeLists.txt file to include the directory containing the headers, 'pthread' was added to the TARGET\_LINK_LIBRARIES command so that the program could compile.

### yaml-cpp
```bash
$ git clone https://github.com/jbeder/yaml-cpp.git
$ cd <path to the cloned yaml-cpp repository>
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
```