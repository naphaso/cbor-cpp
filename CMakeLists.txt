cmake_minimum_required(VERSION 3.15)
project(cborcpp)

include(cmake/StandardProjectSettings.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_14)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

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
target_link_libraries(cborcpp INTERFACE project_options)
set_property(TARGET cborcpp PROPERTY CXX_STANDARD 14)
add_executable(testing ${SOURCE_FILES} ${TEST_SOURCE_FILES})
target_include_directories(testing PRIVATE src)