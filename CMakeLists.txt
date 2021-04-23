cmake_minimum_required(VERSION 3.15)
project(cborcpp)

include(cmake/StandardProjectSettings.cmake)

set(CMAKE_BUILD_TYPE Release)
set(EXPORT_COMPILE_COMMANDS ON)



set(SOURCE_FILES
    src/encoder.cpp
    src/decoder.cpp
    src/input.cpp
    src/listener_debug.cpp
    src/output.cpp
    )

set(TEST_SOURCE_FILES
    test/tests.cpp
    )

add_library(cborcpp SHARED ${SOURCE_FILES})
set_property(TARGET cborcpp PROPERTY CXX_STANDARD 14)
add_executable(testing ${SOURCE_FILES} ${TEST_SOURCE_FILES})
target_include_directories(testing PRIVATE src)