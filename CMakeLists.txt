cmake_minimum_required(VERSION 3.20)

set(MY_PROJECT_NAME jpl)

project(
    ${MY_PROJECT_NAME}
    LANGUAGES CXX
    VERSION   1.0
)

set(CMAKE_CXX_STANDARD 23)

option(BUILD_TESTS "Build the tests" ON)
add_subdirectory(external)

add_library(${MY_PROJECT_NAME} INTERFACE
    include/jpl/cstddef.hpp
    include/jpl/type_traits.hpp
)

target_include_directories(${MY_PROJECT_NAME}
    INTERFACE include
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()