# Automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(tests tests.cpp)
target_link_libraries(tests PUBLIC cborcpp PRIVATE project_warnings project_options catch_main)
