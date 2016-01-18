cmake_minimum_required(VERSION 2.8)
project(isbn)

include(CheckCXXCompilerFlag)

# set ( CMAKE_VERBOSE_MAKEFILE true)

# GLOBAL CONFIGURATION

CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    message(STATUS "Compiling with c++14 support")
elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "Unsupported compiler needs C++11 support!")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -s -O3 -ffast-math -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")

option(INSTALL_LIBISBN "Install libisbn (C++ headers and shared library)" OFF)
option(ENABLE_LTO "Enable LTO optimization" OFF)

if (NOT WIN32)
    set(ENABLE_LTO ON CACHE BOOL "Install libisbn (C++ headers and shared library)")  #LTO was giving me problems on windows
endif ()

if (ENABLE_LTO)
    CHECK_CXX_COMPILER_FLAG("-flto" HAS_LTO_FLAG)
    if (HAS_LTO_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    else ()
        message(WARNING "Compiler doesn't support LTO ")
    endif ()
endif ()


# Set a default build type if none was specified
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif ()


set(PACKAGE_VERSION "0.1")

#LIBISBN
include_directories("libisbn/include/")

set(LIBISBN_INCLUDE_FILES libisbn/include/libisbn.hpp)

set(LIBISBN_SOURCE_FILES libisbn/libisbn.cpp ${LIBISBN_INCLUDE_FILES})

add_library(libisbn SHARED ${LIBISBN_SOURCE_FILES})

set_target_properties(libisbn PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/libisbn")
if (INSTALL_LIBISBN)
    install(TARGETS libisbn DESTINATION lib)
    install(FILES libisbn/include/libisbn.hpp DESTINATION include)
endif ()
# /LIBISBN

#PYTHON BINDINGS
find_program(PYTHON "python")
if (PYTHON)

    set(PYBIND11_PYTHON_VERSION "" CACHE STRING "Python version to use")
    set(Python_ADDITIONAL_VERSIONS 3.4 3.5 3.6)
    find_package(PythonInterp ${PYBIND11_PYTHON_VERSION} REQUIRED)
    find_package(PythonLibs ${PYBIND11_PYTHON_VERSION} REQUIRED)
    include_directories("pylibisbn/pybind11/include")
    include_directories(${PYTHON_INCLUDE_DIR})

    set(PYBIND11_HEADERS
            pylibisbn/pybind11/include/pybind11/pybind11.h
            )

    add_library(pylibisbn SHARED
            ${PYBIND11_HEADERS}
            ${LIBISBN_INCLUDE_FILES}
            pylibisbn/pylibisbn.cpp)

    target_link_libraries(pylibisbn libisbn)

    set(PYLIBISBN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/pylibisbn")

    set_target_properties(pylibisbn PROPERTIES
            PREFIX ""
            LIBRARY_OUTPUT_DIRECTORY ${PYLIBISBN_OUTPUT_DIRECTORY}
            COMPILE_FLAGS "-fvisibility=hidden -Wno-effc++"
            )

    if (WIN32)


        # .PYD file extension on Windows
        set_target_properties(pylibisbn PROPERTIES SUFFIX ".pyd")

        # Link against the Python shared library
        target_link_libraries(pylibisbn ${PYTHON_LIBRARY})
    elseif (UNIX)
        # It's quite common to have multiple copies of the same Python version
        # installed on one's system. E.g.: one copy from the OS and another copy
        # that's statically linked into an application like Blender or Maya.
        # If we link our plugin library against the OS Python here and import it
        # into Blender or Maya later on, this will cause segfaults when multiple
        # conflicting Python instances are active at the same time.

        # Windows is not affected by this issue since it handles DLL imports
        # differently. The solution for Linux and Mac OS is simple: we just don't
        # link against the Python library. The resulting shared library will have
        # missing symbols, but that's perfectly fine -- they will be resolved at
        # import time.

        # .SO file extension on Linux/Mac OS
        set_target_properties(pylibisbn PROPERTIES SUFFIX ".so")
    endif ()


endif ()

#/PYTHON BINDINGS


#UNIT TESTING

set(TESTING_FILES tests/tests.cpp tests/Catch/single_include/catch.hpp)
add_executable(unittest ${TESTING_FILES} libisbn/include/libisbn.hpp)
target_link_libraries(unittest libisbn)
enable_testing()
add_test(testLibIsbn unittest)

# /UNIT TESTING