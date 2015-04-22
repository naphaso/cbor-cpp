cmake_minimum_required(VERSION 2.8)
project(cborcpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)

set(SOURCE_FILES
    src/encoder.cpp
    src/decoder.cpp
    src/input.cpp
    src/listener_debug.cpp
    src/output_dynamic.cpp
    src/output_static.cpp
    )

set(TEST_SOURCE_FILES
    src/tests.cpp
    )

add_library(cborcpp SHARED ${SOURCE_FILES})
add_executable(testing ${SOURCE_FILES} ${TEST_SOURCE_FILES})
