cmake_minimum_required(VERSION 2.8)
project(cborcpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)

file(GLOB HEADERS "include/cborcpp/*.h")

set(SOURCE_FILES
    src/cborcpp/encoder.cpp
    src/cborcpp/decoder.cpp
    src/cborcpp/input.cpp
    src/cborcpp/output_dynamic.cpp
    src/cborcpp/output_static.cpp
    src/cborcpp/cbor_object.cpp
    src/cborcpp/output.cpp
)

set(TEST_SOURCE_FILES
    src/tests.cpp
    )
include_directories(include/)

add_library(cborcpp_dynamic SHARED ${SOURCE_FILES} ${HEADERS})
add_library(cborcpp STATIC ${SOURCE_FILES} ${HEADERS})
add_executable(testing ${SOURCE_FILES} ${HEADERS} ${TEST_SOURCE_FILES})

target_include_directories( cborcpp 
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
