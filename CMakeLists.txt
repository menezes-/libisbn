cmake_minimum_required(VERSION 3.3)
project(isbn)


# set ( CMAKE_VERBOSE_MAKEFILE true )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Weffc++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -s -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/libisbn")

set(SOURCE_FILES main.cpp)


add_subdirectory(libisbn)



add_executable(isbn ${SOURCE_FILES})

target_link_libraries(isbn libisbn)


set_target_properties(isbn PROPERTIES OUTPUT_NAME "main-${CMAKE_BUILD_TYPE}")