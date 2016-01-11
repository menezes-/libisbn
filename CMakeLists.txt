cmake_minimum_required(VERSION 3.3)
project(isbn)


# set ( CMAKE_VERBOSE_MAKEFILE true)

# GLOBAL CONFIGURATION
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Weffc++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -s -O3 -DNDEBUG -flto")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/}")



#LIBISBN

set(LIBISBN_SOURCE_FILES libisbn/libisbn.cpp libisbn/libisbn.hpp)

add_library(libisbn SHARED ${LIBISBN_SOURCE_FILES})

set_target_properties(libisbn PROPERTIES
                      PREFIX ""
                      LIBRARY_OUTPUT_DIRECTORY "CMAKE_RUNTIME_OUTPUT_DIRECTORY/libisbn")

install(TARGETS libisbn DESTINATION lib)
install(FILES libisbn/libisbn.hpp DESTINATION include)
# /LIBISBN



# DEVELOPMENT MAIN
set(SOURCE_FILES main.cpp)
add_executable(isbn ${SOURCE_FILES})
target_link_libraries(isbn libisbn)
set_target_properties(isbn PROPERTIES OUTPUT_NAME "main-${CMAKE_BUILD_TYPE}")
# / DEVELOPMENT MAIN
