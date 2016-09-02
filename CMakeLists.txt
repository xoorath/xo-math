cmake_minimum_required(VERSION 2.8)

project(dkm)

set(DEBUG_FLAGS "-std=c++11 -O0 -msse4.2")
set(RELEASE_FLAGS "-std=c++11 -O0 -msse4.2")

set(CMAKE_CXX_FLAGS ${RELEASE_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()

add_subdirectory(xo-math)